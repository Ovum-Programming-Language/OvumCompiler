#include "StateFieldDecl.hpp"

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateFieldDecl::Name() const {
  return "FieldDecl";
}

IState::StepResult StateFieldDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  // This state is not used - field declarations are handled directly in StateClassMember
  return std::unexpected(StateError(std::string_view("StateFieldDecl should not be called")));
}

}  // namespace ovum::compiler::parser
