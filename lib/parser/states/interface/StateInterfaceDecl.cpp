#include "StateInterfaceDecl.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/types/Param.hpp"
#include "type_parser/ITypeParser.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts, const bool skip_newlines = true) {
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

bool IsIdentifier(const Token& token) {
  const MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(const ContextParser& ctx,
                           ITokenStream& ts,
                           const std::string_view code,
                           const std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error(code, message, span);
      } else {
        ctx.Diags()->Error(code, message);
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

std::unique_ptr<TypeReference> ParseType(const ContextParser& ctx, ITokenStream& ts) {
  if (ctx.TypeParser() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPE_PARSER", "type parser not available");
    }
    return nullptr;
  }
  return ctx.TypeParser()->ParseType(ts, *ctx.Diags());
}

void ConsumeTerminators(ITokenStream& ts) {
  SkipTrivia(ts, false);
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    if (const std::string type = t.GetStringType(); type == "NEWLINE") {
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

} // namespace

std::string_view StateInterfaceDecl::Name() const {
  return "InterfaceDecl";
}

IState::StepResult StateInterfaceDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  auto* interface_decl = ctx.TopNodeAs<InterfaceDecl>();
  if (interface_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected InterfaceDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in interface declaration")));
  }

  const Token& start = ts.Peek();
  const std::string lex = start.GetLexeme();
  SourceSpan span = SpanFrom(start);

  // Check for call
  if (lex == "call") {
    ts.Consume();
    SkipTrivia(ts);

    if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_CALL_PARAMS_OPEN", std::string_view("expected '(' after 'call'"));
      }
      return std::unexpected(StateError(std::string_view("expected '(' after 'call'")));
    }
    ts.Consume();

    // Parse parameters
    std::vector<Param> params;
    SkipTrivia(ts);
    if (!ts.IsEof() && ts.Peek().GetLexeme() != ")") {
      while (true) {
        if (!ts.IsEof() && ts.Peek().GetLexeme() == "var") {
          ts.Consume();
          SkipTrivia(ts);
        }

        std::string name = ReadIdentifier(ctx, ts, "P_PARAM_NAME", "expected parameter name");
        if (name.empty()) {
          break;
        }

        SkipTrivia(ts);
        if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
          if (ctx.Diags() != nullptr) {
            ctx.Diags()->Error("P_PARAM_COLON", "expected ':' after parameter name");
          }
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
        SkipTrivia(ts);
      }
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_CALL_PARAMS_CLOSE", std::string_view("expected ')' after parameters"));
      }
      return std::unexpected(StateError(std::string_view("expected ')' after parameters")));
    }
    ts.Consume();

    std::unique_ptr<TypeReference> return_type = nullptr;
    SkipTrivia(ts);
    if (!ts.IsEof() && ts.Peek().GetLexeme() == ":") {
      ts.Consume();
      SkipTrivia(ts);
      return_type = ParseType(ctx, ts);
    }

    auto method = ctx.Factory()->MakeInterfaceMethod("call", std::move(params), std::move(return_type), span);
    interface_decl->AddMember(std::move(method));
    ConsumeTerminators(ts);
    return false;
  }

  // Method declaration (fun)
  if (lex != "fun") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_INTERFACE_METHOD", std::string_view("expected 'fun' or 'call' in interface"));
    }
    return std::unexpected(StateError(std::string_view("expected 'fun' or 'call' in interface")));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts, "P_METHOD_NAME", "expected method name");
  if (name.empty()) {
    return std::unexpected(StateError(std::string_view("expected method name")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_METHOD_PARAMS_OPEN", std::string_view("expected '(' after method name"));
    }
    return std::unexpected(StateError(std::string_view("expected '(' after method name")));
  }
  ts.Consume();

  // Parse parameters
  std::vector<Param> params;
  SkipTrivia(ts);
  if (!ts.IsEof() && ts.Peek().GetLexeme() != ")") {
    while (true) {
      if (!ts.IsEof() && ts.Peek().GetLexeme() == "var") {
        ts.Consume();
        SkipTrivia(ts);
      }

      std::string param_name = ReadIdentifier(ctx, ts, "P_PARAM_NAME", "expected parameter name");
      if (param_name.empty()) {
        break;
      }

      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
        if (ctx.Diags() != nullptr) {
          ctx.Diags()->Error("P_PARAM_COLON", "expected ':' after parameter name");
        }
        break;
      }
      ts.Consume();

      SkipTrivia(ts);
      auto type = ParseType(ctx, ts);
      if (type == nullptr) {
        break;
      }

      params.emplace_back(param_name, std::move(*type));

      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != ",") {
        break;
      }
      ts.Consume();
      SkipTrivia(ts);
    }
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_METHOD_PARAMS_CLOSE", std::string_view("expected ')' after parameters"));
    }
    return std::unexpected(StateError(std::string_view("expected ')' after parameters")));
  }
  ts.Consume();

  std::unique_ptr<TypeReference> return_type = nullptr;
  SkipTrivia(ts);
  if (!ts.IsEof() && ts.Peek().GetLexeme() == ":") {
    ts.Consume();
    SkipTrivia(ts);
    return_type = ParseType(ctx, ts);
  }

  if (ts.LastConsumed() != nullptr) {
    span = Union(span, SpanFrom(*ts.LastConsumed()));
  }
  auto method = ctx.Factory()->MakeInterfaceMethod(std::move(name), std::move(params), std::move(return_type), span);
  interface_decl->AddMember(std::move(method));
  ConsumeTerminators(ts);
  return false;
}

} // namespace ovum::compiler::parser
