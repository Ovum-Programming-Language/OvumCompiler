#include "StateIfHead.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
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
      ctx.Diags()->Error("P_EXPR_PARSER", "expression parser not available");
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

} // namespace

std::string_view StateIfHead::Name() const {
  return "IfHead";
}

IState::StepResult StateIfHead::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in if statement")));
  }

  const Token& start = ts.Peek();

  // Check if we already have an IfStmt on stack
  IfStmt* if_stmt = ctx.TopNodeAs<IfStmt>();
  if (if_stmt == nullptr) {
    // Create new IfStmt
    auto new_if = ctx.Factory()->MakeIfStmt({}, nullptr, StateBase::SpanFrom(start));
    ctx.PushNode(std::unique_ptr<AstNode>(new_if.get()));
    if_stmt = new_if.get();
  }

  // Parse condition
  if (start.GetLexeme() == "if") {
    ts.Consume();
    SkipTrivia(ts);

    if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_IF_COND_OPEN", "expected '(' after 'if'");
      }
      return std::unexpected(StateError(std::string_view("expected '(' after 'if'")));
    }
    ts.Consume();

    SkipTrivia(ts);
    auto condition = ParseExpression(ctx, ts);
    if (condition == nullptr) {
      return std::unexpected(StateError(std::string_view("failed to parse if condition")));
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_IF_COND_CLOSE", "expected ')' after condition");
      }
      return std::unexpected(StateError(std::string_view("expected ')' after condition")));
    }
    ts.Consume();

    SkipTrivia(ts);

    // Parse then block
    if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_IF_BLOCK", "expected '{' for if body");
      }
      return std::unexpected(StateError(std::string_view("expected '{' for if body")));
    }

    ts.Consume();
    auto then_block = ctx.Factory()->MakeBlock({}, SourceSpan{});
    ctx.PushNode(std::unique_ptr<AstNode>(then_block.get()));
    ctx.PushState(StateRegistry::Block());

    // Store condition for later
    ctx.PushNode(std::unique_ptr<AstNode>(condition.release()));
    ctx.PushState(StateRegistry::IfTail());
    return true;
  }

  return std::unexpected(StateError(std::string_view("expected 'if' keyword")));
}

} // namespace ovum::compiler::parser
