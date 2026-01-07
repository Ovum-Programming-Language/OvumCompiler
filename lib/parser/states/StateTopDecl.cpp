#include "StateTopDecl.hpp"

#include <memory>
#include <string>
#include <string_view>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"
#include "lib/parser/tokens/token_traits/MatchType.hpp"
#include "lib/parser/types/Param.hpp"
#include "pratt/IExpressionParser.hpp"
#include "type_parser/ITypeParser.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts, bool skip_newlines = true) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT") {
      ts.Consume();
      continue;
    }
    if (skip_newlines && type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

SourceSpan SpanFrom(const Token& token) {
  return StateBase::SpanFrom(token);
}

SourceSpan Union(const SourceSpan& a, const SourceSpan& b) {
  return StateBase::Union(a, b);
}

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

bool MatchLexeme(ITokenStream& ts, std::string_view lex) {
  if (ts.IsEof()) {
    return false;
  }
  class MatchLexeme matcher(lex);
  if (matcher.TryMatch(ts.Peek())) {
    ts.Consume();
    return true;
  }
  return false;
}

void ReportUnexpected(IDiagnosticSink* diags, std::string_view code, std::string_view message, const Token* tok) {
  if (diags == nullptr) {
    return;
  }
  std::optional<SourceSpan> span;
  if (tok != nullptr) {
    span = SpanFrom(*tok);
  }
  diags->Error(code, message, span);
}

std::string ReadIdentifier(ContextParser& ctx, ITokenStream& ts, std::string_view code, std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    const Token* tok = ts.TryPeek();
    ReportUnexpected(ctx.Diags(), code, message, tok);
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

std::unique_ptr<TypeReference> ParseType(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.TypeParser() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPE_PARSER", std::string_view("type parser not available"));
    }
    return nullptr;
  }
  return ctx.TypeParser()->ParseType(ts, *ctx.Diags());
}

std::unique_ptr<Expr> ParseExpression(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.Expr() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_EXPR_PARSER", std::string_view("expression parser not available"));
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

void ConsumeTerminators(ITokenStream& ts) {
  SkipTrivia(ts, false);
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    if (t.GetLexeme() == ";") {
      ts.Consume();
      continue;
    }
    break;
  }
}

std::vector<Param> ParseParamList(ContextParser& ctx, ITokenStream& ts) {
  std::vector<Param> params;
  SkipTrivia(ts);

  if (ts.IsEof() || ts.Peek().GetLexeme() == ")") {
    return params;
  }

  while (true) {
    SkipTrivia(ts);
    bool is_var = false;
    if (!ts.IsEof() && ts.Peek().GetLexeme() == "var") {
      ts.Consume();
      is_var = true;
      SkipTrivia(ts);
    }

    std::string name = ReadIdentifier(ctx, ts, "P_PARAM_NAME", "expected parameter name");
    if (name.empty()) {
      break;
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
      ReportUnexpected(ctx.Diags(), "P_PARAM_COLON", "expected ':' after parameter name", ts.TryPeek());
      break;
    }
    ts.Consume();

    SkipTrivia(ts);
    auto type = ParseType(ctx, ts);
    if (type == nullptr) {
      break;
    }

    params.emplace_back(name, std::move(*type));

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ",") {
      break;
    }
    ts.Consume();
  }

  return params;
}

std::unique_ptr<Block> ParseBlock(ContextParser& ctx, ITokenStream& ts) {
  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    const Token* tok = ts.TryPeek();
    ReportUnexpected(ctx.Diags(), "P_BLOCK_OPEN", "expected '{'", tok);
    return nullptr;
  }
  ts.Consume();

  auto block = ctx.Factory()->MakeBlock({}, SourceSpan{});
  ctx.PushNode(std::move(block));
  ctx.PushState(StateRegistry::Block());
  return nullptr; // Will be popped later
}

std::unique_ptr<Block> WrapSingleStmt(ContextParser& ctx, std::unique_ptr<Stmt> stmt) {
  std::vector<std::unique_ptr<Stmt>> stmts;
  stmts.push_back(std::move(stmt));
  SourceSpan span = stmt ? stmt->Span() : SourceSpan{};
  return ctx.Factory()->MakeBlock(std::move(stmts), span);
}

} // namespace

std::string_view StateTopDecl::Name() const {
  return "TopDecl";
}

IState::StepResult StateTopDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return false;
  }

  Module* module = ctx.TopNodeAs<Module>();
  if (module == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Module node on stack")));
  }

  const Token& start = ts.Peek();
  std::string lex = start.GetLexeme();
  SourceSpan span = SpanFrom(start);

  // Check for pure fun
  bool is_pure = false;
  if (lex == "pure") {
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      ReportUnexpected(ctx.Diags(), "P_PURE_FUN", "expected 'fun' after 'pure'", ts.TryPeek());
      return std::unexpected(StateError(std::string_view("expected 'fun' after 'pure'")));
    }
    if (ts.Peek().GetLexeme() != "fun") {
      ReportUnexpected(ctx.Diags(), "P_PURE_FUN", "expected 'fun' after 'pure'", &ts.Peek());
      return std::unexpected(StateError(std::string_view("expected 'fun' after 'pure'")));
    }
    is_pure = true;
    lex = "fun";
  }

  if (lex == "fun") {
    ctx.PushState(StateRegistry::FuncHdr());
    return true;
  }

  if (lex == "class") {
    ctx.PushState(StateRegistry::ClassHdr());
    return true;
  }

  if (lex == "interface") {
    ctx.PushState(StateRegistry::InterfaceHdr());
    return true;
  }

  if (lex == "typealias") {
    ctx.PushState(StateRegistry::TypeAliasDecl());
    return true;
  }

  if (lex == "var" || lex == "val") {
    bool is_var = (lex == "var");
    ts.Consume();
    SkipTrivia(ts);

    std::string name = ReadIdentifier(ctx, ts, "P_GLOBAL_VAR_NAME", "expected variable name");
    if (name.empty()) {
      return std::unexpected(StateError(std::string_view("expected variable name")));
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
      ReportUnexpected(ctx.Diags(), "P_GLOBAL_VAR_COLON", "expected ':' after variable name", ts.TryPeek());
      return std::unexpected(StateError(std::string_view("expected ':' after variable name")));
    }
    ts.Consume();

    SkipTrivia(ts);
    auto type = ParseType(ctx, ts);
    if (type == nullptr) {
      return std::unexpected(StateError(std::string_view("failed to parse type")));
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "=") {
      ReportUnexpected(ctx.Diags(), "P_GLOBAL_VAR_INIT", "expected '=' for global variable", ts.TryPeek());
      return std::unexpected(StateError(std::string_view("expected '=' for global variable")));
    }
    ts.Consume();

    SkipTrivia(ts);
    auto init = ParseExpression(ctx, ts);
    if (init == nullptr) {
      return std::unexpected(StateError(std::string_view("failed to parse initialization expression")));
    }

    span = Union(span, init->Span());
    auto decl = ctx.Factory()->MakeGlobalVar(is_var, std::move(name), std::move(*type), std::move(init), span);
    module->AddDecl(std::move(decl));

    ConsumeTerminators(ts);
    return true;
  }

  const Token* tok = ts.TryPeek();
  ReportUnexpected(ctx.Diags(), "P_TOP_DECL", std::string_view("expected top-level declaration"), tok);
  if (tok != nullptr) {
    ts.Consume();
  }
  return true;
}

} // namespace ovum::compiler::parser
