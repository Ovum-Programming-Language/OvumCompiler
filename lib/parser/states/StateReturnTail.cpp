#include "StateReturnTail.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
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

std::string_view StateReturnTail::Name() const {
  return "ReturnTail";
}

IState::StepResult StateReturnTail::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in return statement")));
  }

  const Token& start = ts.Peek();
  SourceSpan span = StateBase::SpanFrom(start);

  SkipTrivia(ts);

  // Check if there's a return value
  if (ts.IsEof() || (ts.Peek().GetLexeme() == ";" || ts.Peek().GetStringType() == "NEWLINE")) {
    // Return without value
    auto stmt = ctx.Factory()->MakeReturnStmt(nullptr, span);
    block->Append(std::move(stmt));
    ConsumeTerminators(ts);
    return false;
  }

  // Return with value
  auto expr = ParseExpression(ctx, ts);
  if (expr == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse return expression")));
  }

  span = StateBase::Union(span, expr->Span());
  auto stmt = ctx.Factory()->MakeReturnStmt(std::move(expr), span);
  block->Append(std::move(stmt));
  ConsumeTerminators(ts);
  return false;
}

} // namespace ovum::compiler::parser
