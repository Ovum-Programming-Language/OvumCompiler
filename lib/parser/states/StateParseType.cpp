#include "StateParseType.hpp"

#include <memory>

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateParseType::Name() const {
  return "ParseType";
}

IState::StepResult StateParseType::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  if (ctx.TypeParser() == nullptr) {
    return std::unexpected(StateError("type parser not available"));
  }

  auto type = ctx.TypeParser()->ParseType(ts, *ctx.Diags());
  if (type == nullptr) {
    return std::unexpected(StateError("failed to parse type"));
  }

  // TypeReference is not an AstNode, so we can't push it to the stack
  // This state is typically used in contexts where the type is consumed immediately
  return false;
}

}  // namespace ovum::compiler::parser
