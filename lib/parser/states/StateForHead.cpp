#include "StateForHead.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "pratt/IExpressionParser.hpp"

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

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(ContextParser& ctx, ITokenStream& ts, std::string_view code, std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      const Token* tok = ts.TryPeek();
      if (tok != nullptr) {
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

std::unique_ptr<Expr> ParseExpression(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.Expr() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_EXPR_PARSER", "expression parser not available");
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

} // namespace

std::string_view StateForHead::Name() const {
  return "ForHead";
}

IState::StepResult StateForHead::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  auto* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in for statement")));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "for") {
    return std::unexpected(StateError(std::string_view("expected 'for' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FOR_OPEN", "expected '(' after 'for'");
    }
    return std::unexpected(StateError(std::string_view("expected '(' after 'for'")));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string iter_name = ReadIdentifier(ctx, ts, "P_FOR_ITER", "expected iterator name");
  if (iter_name.empty()) {
    return std::unexpected(StateError(std::string_view("expected iterator name")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "in") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FOR_IN", "expected 'in' after iterator name");
    }
    return std::unexpected(StateError(std::string_view("expected 'in' after iterator name")));
  }
  ts.Consume();

  SkipTrivia(ts);
  auto iter_expr = ParseExpression(ctx, ts);
  if (iter_expr == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse iterator expression")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FOR_CLOSE", "expected ')' after iterator expression");
    }
    return std::unexpected(StateError(std::string_view("expected ')' after iterator expression")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FOR_BLOCK", "expected '{' for for body");
    }
    return std::unexpected(StateError(std::string_view("expected '{' for for body")));
  }

  ts.Consume();
  auto body = ctx.Factory()->MakeBlock({}, SourceSpan{});

  SourceSpan span = StateBase::Union(SpanFrom(start), iter_expr->Span());
  // Pass nullptr for body - it will be set later in StateBlock
  auto for_stmt = ctx.Factory()->MakeForStmt(std::move(iter_name), std::move(iter_expr), nullptr, span);

  // Push for_stmt first, then body, so StateBlock can find for_stmt as parent
  ctx.PushNode(std::move(for_stmt));
  ctx.PushNode(std::move(body));
  ctx.PopState(); // Pop ForHead before pushing Block
  ctx.PushState(StateRegistry::Block());
  return true;
}

} // namespace ovum::compiler::parser
