#include "StateFuncHdr.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT") {
      ts.Consume();
      continue;
    }
    if (type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(const ContextParser& ctx, ITokenStream& ts) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error("P_FUN_NAME", std::string_view("expected function name"), span);
      } else {
        ctx.Diags()->Error("P_FUN_NAME", std::string_view("expected function name"));
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

} // namespace

std::string_view StateFuncHdr::Name() const {
  return "FuncHdr";
}

IState::StepResult StateFuncHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in function header")));
  }

  // Check if we already have a function node
  if (const auto* func = ctx.TopNodeAs<FunctionDecl>(); func == nullptr) {
    // Create new function node
    bool is_pure = false;
    const Token& start = ts.Peek();

    // Check for 'pure' keyword (should have been consumed by StateTopDecl)
    // But we handle it here too for safety
    if (start.GetLexeme() == "pure") {
      ts.Consume();
      is_pure = true;
      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != "fun") {
        if (ctx.Diags() != nullptr) {
          ctx.Diags()->Error("P_PURE_FUN", std::string_view("expected 'fun' after 'pure'"));
        }
        return std::unexpected(StateError(std::string_view("expected 'fun' after 'pure'")));
      }
    }

    if (ts.Peek().GetLexeme() != "fun") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_FUN_KEYWORD", std::string_view("expected 'fun' keyword"));
      }
      return std::unexpected(StateError(std::string_view("expected 'fun' keyword")));
    }
    ts.Consume();

    SkipTrivia(ts);
    std::string name = ReadIdentifier(ctx, ts);
    if (name.empty()) {
      return std::unexpected(StateError(std::string_view("expected function name")));
    }

    SourceSpan span = SpanFrom(start);
    auto func_node = ctx.Factory()->MakeFunction(is_pure, std::move(name), {}, nullptr, nullptr, span);
    ctx.PushNode(std::move(func_node));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FUN_PARAMS_OPEN", std::string_view("expected '(' after function name"));
    }
    return std::unexpected(StateError(std::string_view("expected '(' after function name")));
  }

  if (ts.Peek().GetLexeme() == "(") {
    ts.Consume();
  }

  ctx.PopState();
  ctx.PushState(StateRegistry::FuncParams());
  return true;
}

} // namespace ovum::compiler::parser
