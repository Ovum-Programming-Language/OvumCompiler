#include "StateGlobalVarDecl.hpp"

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateGlobalVarDecl::Name() const {
  return "GlobalVarDecl";
}

IState::StepResult StateGlobalVarDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  // This state is not used - global variable declarations are handled directly in StateTopDecl
  return std::unexpected(StateError("StateGlobalVarDecl should not be called"));
}

}  // namespace ovum::compiler::parser
