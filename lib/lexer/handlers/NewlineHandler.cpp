#include "NewlineHandler.hpp"

#include "lib/tokens/TokenFactory.hpp"

namespace ovum::compiler::lexer {

OptToken NewlineHandler::Scan(SourceCodeWrapper& wrapper) {
  return std::make_optional(TokenFactory::MakeNewline(wrapper.GetLine() - 1, wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
