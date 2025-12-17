#include "StateWhileHead.hpp"

#include <memory>

#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

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
      ctx.Diags()->Error("P_EXPR_PARSER", "expression parser not available");
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

}  // namespace

std::string_view StateWhileHead::Name() const {
  return "WhileHead";
}

IState::StepResult StateWhileHead::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError("expected Block node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in while statement"));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "while") {
    return std::unexpected(StateError("expected 'while' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_COND_OPEN", "expected '(' after 'while'");
    }
    return std::unexpected(StateError("expected '(' after 'while'"));
  }
  ts.Consume();

  SkipTrivia(ts);
  auto condition = ParseExpression(ctx, ts);
  if (condition == nullptr) {
    return std::unexpected(StateError("failed to parse while condition"));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_COND_CLOSE", "expected ')' after condition");
    }
    return std::unexpected(StateError("expected ')' after condition"));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_WHILE_BLOCK", "expected '{' for while body");
    }
    return std::unexpected(StateError("expected '{' for while body"));
  }

  ts.Consume();
  auto body = ctx.Factory()->MakeBlock({}, SourceSpan{});
  ctx.PushNode(std::unique_ptr<AstNode>(body.get()));

  SourceSpan span = StateBase::Union(StateBase::SpanFrom(start), condition->Span());
  auto while_stmt = ctx.Factory()->MakeWhileStmt(std::move(condition), std::unique_ptr<Block>(body), span);
  body.release();

  block->Append(std::move(while_stmt));
  ctx.PushState(StateRegistry::Block());
  return true;
}

}  // namespace ovum::compiler::parser
