#include "StateVarDeclTail.hpp"

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateVarDeclTail::Name() const {
  return "VarDeclTail";
}

IState::StepResult StateVarDeclTail::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  // This state is not used - variable declarations are handled directly in StateStmt
  return std::unexpected(StateError(std::string_view("StateVarDeclTail should not be called")));
}

} // namespace ovum::compiler::parser
