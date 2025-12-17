#include "StateUnsafeBlock.hpp"

#include <memory>

#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
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

}  // namespace

std::string_view StateUnsafeBlock::Name() const {
  return "UnsafeBlock";
}

IState::StepResult StateUnsafeBlock::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError("expected Block node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in unsafe block"));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "unsafe") {
    return std::unexpected(StateError("expected 'unsafe' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_UNSAFE_BLOCK", "expected '{' after 'unsafe'");
    }
    return std::unexpected(StateError("expected '{' after 'unsafe'"));
  }

  ts.Consume();
  auto unsafe_body = ctx.Factory()->MakeBlock({}, SourceSpan{});
  ctx.PushNode(std::unique_ptr<AstNode>(unsafe_body.get()));

  SourceSpan span = StateBase::SpanFrom(start);
  auto unsafe_stmt = ctx.Factory()->MakeUnsafeBlock(std::unique_ptr<Block>(unsafe_body), span);
  unsafe_body.release();

  block->Append(std::move(unsafe_stmt));
  ctx.PushState(StateRegistry::Block());
  return true;
}

}  // namespace ovum::compiler::parser
