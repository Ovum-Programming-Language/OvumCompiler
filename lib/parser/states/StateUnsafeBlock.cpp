#include "StateUnsafeBlock.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

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

} // namespace

std::string_view StateUnsafeBlock::Name() const {
  return "UnsafeBlock";
}

IState::StepResult StateUnsafeBlock::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (const auto* block = ctx.TopNodeAs<Block>(); block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in unsafe block")));
  }

  // The 'unsafe' token was already consumed in StateStmt, so we expect '{' here
  const Token& start = ts.Peek();
  if (start.GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_UNSAFE_BLOCK", std::string_view("expected '{' after 'unsafe'"));
    }
    return std::unexpected(StateError(std::string_view("expected '{' after 'unsafe'")));
  }

  ts.Consume();
  auto unsafe_body = ctx.Factory()->MakeBlock({}, SourceSpan{});

  const SourceSpan span = SpanFrom(start);
  auto unsafe_stmt = ctx.Factory()->MakeUnsafeBlock(nullptr, span); // Pass nullptr for body initially

  // Push unsafe_stmt first, then body, so StateBlock can find unsafe_stmt as parent
  ctx.PushNode(std::move(unsafe_stmt));
  ctx.PushNode(std::move(unsafe_body));
  ctx.PopState(); // Pop UnsafeBlock before pushing Block
  ctx.PushState(StateRegistry::Block());
  return true;
}

} // namespace ovum::compiler::parser
