#include "PrattExpressionParser.hpp"

#include <optional>
#include <string>
#include <utility>

#include <tokens/Token.hpp>
#include "lib/parser/ast/nodes/exprs/tags/OpTags.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/pratt/specifications/InfixSpec.hpp"
#include "lib/parser/pratt/specifications/PostfixSpec.hpp"

namespace ovum::compiler::parser {

namespace {

constexpr int kBP_Assign = 5; // right-assoc
constexpr int kBP_Elvis = 20; // right-assoc
constexpr int kBP_Post = 100; // postfix

bool Lex(const Token& t, std::string_view s) {
  return t.GetLexeme() == s;
}

bool IsIdentifier(const Token& t) {
  return t.GetStringType() == "IDENT";
}

bool IsLiteral(const Token& t) {
  const std::string ty = t.GetStringType();
  return ty.rfind("LITERAL", 0) == 0; // "LITERAL:*"
}

} // namespace

PrattExpressionParser::PrattExpressionParser(std::unique_ptr<IOperatorResolver> resolver,
                                             std::shared_ptr<IAstFactory> factory) :
    resolver_(std::move(resolver)), factory_(std::move(factory)) {
}

std::unique_ptr<Expr> PrattExpressionParser::Parse(ITokenStream& ts, IDiagnosticSink& diags) {
  return ParseExpr(ts, diags, 0);
}

std::unique_ptr<Expr> PrattExpressionParser::ParseExpr(ITokenStream& ts, IDiagnosticSink& diags, int min_bp) {
  std::unique_ptr<Expr> left = ParsePrefix(ts, diags);
  if (!left) {
    return nullptr;
  }

  while (!ts.IsEof()) {
    const Token& look = ts.Peek();
    const auto post = resolver_->FindPostfix(look);
    if (!post.has_value()) {
      break;
    }

    const int bp = post->get().BindingPower();
    if (bp < min_bp) {
      break;
    }

    left = ParsePostfix(ts, diags, std::move(left));
    if (!left) {
      return nullptr;
    }
  }

  while (!ts.IsEof()) {
    const Token& look = ts.Peek();

    const bool is_ref_assign = Lex(look, "=");
    const bool is_copy_assign = Lex(look, ":=");
    if (is_ref_assign || is_copy_assign) {
      if (kBP_Assign < min_bp) {
        break;
      }

      ts.Consume();

      std::unique_ptr<Expr> rhs = ParseExpr(ts, diags, kBP_Assign - 1); // right-assoc
      if (!rhs) {
        return nullptr;
      }

      left = factory_->MakeAssign(
          is_ref_assign ? OpTags::RefAssign() : OpTags::CopyAssign(), std::move(left), std::move(rhs));

      while (!ts.IsEof()) {
        const Token& next = ts.Peek();
        const auto post2 = resolver_->FindPostfix(next);
        if (!post2.has_value() || post2->get().BindingPower() < min_bp) {
          break;
        }

        left = ParsePostfix(ts, diags, std::move(left));
        if (!left) {
          return nullptr;
        }
      }
      continue;
    }

    const auto inf = resolver_->FindInfix(look);
    if (!inf.has_value()) {
      break;
    }

    const int lbp = inf->get().Lbp();
    if (lbp < min_bp) {
      break;
    }

    if (inf->get().IsElvis()) {
      ts.Consume(); // ?:
      std::unique_ptr<Expr> rhs = ParseExpr(ts, diags, inf->get().Rbp());
      if (!rhs) {
        return nullptr;
      }

      left = factory_->MakeElvis(std::move(left), std::move(rhs));
    } else {
      const IBinaryOpTag* tag = inf->get().Tag();
      if (tag == nullptr) {
        diags.Error("E_EXPR_OP", "internal: infix without tag");
        return nullptr;
      }

      ts.Consume();

      const int next_min = inf->get().IsRightAssociative() ? inf->get().Rbp() : inf->get().Rbp() + 1;

      std::unique_ptr<Expr> rhs = ParseExpr(ts, diags, next_min);
      if (!rhs) {
        return nullptr;
      }

      left = factory_->MakeBinary(*tag, std::move(left), std::move(rhs));
    }

    while (!ts.IsEof()) {
      const Token& next2 = ts.Peek();
      const auto post3 = resolver_->FindPostfix(next2);
      if (!post3.has_value() || post3->get().BindingPower() < min_bp) {
        break;
      }

      left = ParsePostfix(ts, diags, std::move(left));
      if (!left) {
        return nullptr;
      }
    }
  }

  return left;
}

std::unique_ptr<Expr> PrattExpressionParser::ParsePrefix(ITokenStream& ts, IDiagnosticSink& diags) {
  if (ts.IsEof()) {
    diags.Error("E_EXPR_EOF", "unexpected end of input");
    return nullptr;
  }

  const Token& look = ts.Peek();

  if (auto pre = resolver_->FindPrefix(look)) {
    ts.Consume();
    std::unique_ptr<Expr> operand = ParseExpr(ts, diags, kBP_Post - 1);
    if (!operand) {
      return nullptr;
    }

    return factory_->MakeUnary(pre->get(), std::move(operand));
  }

  if (Lex(look, "(")) {
    ts.Consume();
    std::unique_ptr<Expr> inner = ParseExpr(ts, diags, 0);
    if (!inner) {
      return nullptr;
    }

    if (ts.IsEof() || !Lex(ts.Peek(), ")")) {
      diags.Error("E_EXPR_GROUP", "expected ')'");
      return nullptr;
    }

    ts.Consume();
    return inner;
  }

  if (IsIdentifier(look)) {
    std::string name = look.GetLexeme();
    ts.Consume();
    return factory_->MakeIdentRef(std::move(name));
  }

  if (IsLiteral(look)) {
    std::unique_ptr<Expr> lit = factory_->MakeLiteralFromToken(look);
    ts.Consume();
    return lit;
  }

  diags.Error("E_EXPR_PRIMARY", "expected primary expression");
  return nullptr;
}

std::unique_ptr<Expr> PrattExpressionParser::ParsePostfix(ITokenStream& ts,
                                                          IDiagnosticSink& diags,
                                                          std::unique_ptr<Expr> base) {
  if (!base) {
    return nullptr;
  }

  if (ts.IsEof()) {
    return base;
  }

  const Token& look = ts.Peek();

  if (Lex(look, "(")) {
    ts.Consume();
    auto args = ParseArgList(ts, diags, ')');
    if (args.empty() && (ts.IsEof() || !Lex(ts.Peek(), ")"))) {
      diags.Error("E_CALL_CLOSE", "expected ')'");
      return nullptr;
    }

    if (!ts.IsEof() && Lex(ts.Peek(), ")")) {
      ts.Consume();
    }

    return factory_->MakeCall(std::move(base), std::move(args));
  }

  if (Lex(look, ".")) {
    ts.Consume();

    if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
      diags.Error("E_DOT_IDENT", "expected identifier after '.'");
      return nullptr;
    }

    std::string member = ts.Peek().GetLexeme();
    ts.Consume();

    if (!ts.IsEof() && Lex(ts.Peek(), "(")) {
      ts.Consume();
      auto args = ParseArgList(ts, diags, ')');
      if (args.empty() && (ts.IsEof() || !Lex(ts.Peek(), ")"))) {
        diags.Error("E_CALL_CLOSE", "expected ')'");
        return nullptr;
      }

      if (!ts.IsEof() && Lex(ts.Peek(), ")")) {
        ts.Consume();
      }

      auto callee = factory_->MakeFieldAccess(std::move(base), std::move(member));
      return factory_->MakeCall(std::move(callee), std::move(args));
    }

    return factory_->MakeFieldAccess(std::move(base), std::move(member));
  }

  if (Lex(look, "?.")) {
    ts.Consume();

    if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
      diags.Error("E_SAFECALL_IDENT", "expected identifier after '?.'");
      return nullptr;
    }

    std::string method = ts.Peek().GetLexeme();
    ts.Consume();

    std::vector<std::unique_ptr<Expr>> args;
    if (!ts.IsEof() && Lex(ts.Peek(), "(")) {
      ts.Consume();
      args = ParseArgList(ts, diags, ')');

      if (args.empty() && (ts.IsEof() || !Lex(ts.Peek(), ")"))) {
        diags.Error("E_CALL_CLOSE", "expected ')'");
        return nullptr;
      }

      if (!ts.IsEof() && Lex(ts.Peek(), ")")) {
        ts.Consume();
      }
    }

    return factory_->MakeSafeCall(std::move(base), std::move(method), std::move(args));
  }

  if (Lex(look, "::")) {
    ts.Consume();
    if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
      diags.Error("E_NS_IDENT", "expected identifier after '::'");
      return nullptr;
    }

    std::string ident = ts.Peek().GetLexeme();
    ts.Consume();
    return factory_->MakeNamespaceRef(std::move(base), std::move(ident));
  }

  if (Lex(look, "as") || Lex(look, "is")) {
    diags.Error("E_TYPE_POSTFIX", "type postfix ('as'/'is') requires type parser");
    return nullptr;
  }

  return base;
}

std::vector<std::unique_ptr<Expr>> PrattExpressionParser::ParseArgList(ITokenStream& ts,
                                                                       IDiagnosticSink& diags,
                                                                       char closing) {
  std::vector<std::unique_ptr<Expr>> result;

  if (!ts.IsEof() && ts.Peek().GetLexeme().size() == 1 && ts.Peek().GetLexeme()[0] == closing) {
    return result;
  }

  if (auto arg = ParseExpr(ts, diags, 0)) {
    result.push_back(std::move(arg));
  } else {
    return {};
  }

  while (!ts.IsEof() && ts.Peek().GetLexeme() == ",") {
    ts.Consume();
    auto arg = ParseExpr(ts, diags, 0);

    if (!arg) {
      return {};
    }

    result.push_back(std::move(arg));
  }

  return result;
}

std::unique_ptr<Expr> PrattExpressionParser::MakeInfix(const InfixSpec& spec,
                                                       std::unique_ptr<Expr> lhs,
                                                       std::unique_ptr<Expr> rhs) {
  if (spec.IsElvis()) {
    return factory_->MakeElvis(std::move(lhs), std::move(rhs));
  }
  return factory_->MakeBinary(*spec.Tag(), std::move(lhs), std::move(rhs));
}

} // namespace ovum::compiler::parser
