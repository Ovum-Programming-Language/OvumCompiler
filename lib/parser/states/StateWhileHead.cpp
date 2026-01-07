#include "StateWhileHead.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
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

std::unique_ptr<Expr> ParseExpression(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.Expr() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_EXPR_PARSER", std::string_view("expression parser not available"));
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

} // namespace

std::string_view StateWhileHead::Name() const {
  return "WhileHead";
}

IState::StepResult StateWhileHead::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in while statement")));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "while") {
    return std::unexpected(StateError(std::string_view("expected 'while' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_COND_OPEN", std::string_view("expected '(' after 'while'"));
    }
    return std::unexpected(StateError(std::string_view("expected '(' after 'while'")));
  }
  ts.Consume();

  SkipTrivia(ts);
  auto condition = ParseExpression(ctx, ts);
  if (condition == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse while condition")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_COND_CLOSE", std::string_view("expected ')' after condition"));
    }
    return std::unexpected(StateError(std::string_view("expected ')' after condition")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_BLOCK", std::string_view("expected '{' for while body"));
    }
    return std::unexpected(StateError(std::string_view("expected '{' for while body")));
  }

  ts.Consume();
  auto body = ctx.Factory()->MakeBlock({}, SourceSpan{});
  ctx.PushNode(std::move(body));

  SourceSpan span = StateBase::Union(StateBase::SpanFrom(start), condition->Span());
  auto while_stmt = ctx.Factory()->MakeWhileStmt(std::move(condition), std::move(body), span);

  block->Append(std::move(while_stmt));
  ctx.PushState(StateRegistry::Block());
  return true;
}

} // namespace ovum::compiler::parser
