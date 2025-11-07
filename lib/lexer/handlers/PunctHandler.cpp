#include "PunctHandler.hpp"

#include "lib/tokens/TokenFactory.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> PunctHandler::Scan(SourceCodeWrapper& wrapper) {
  char c = wrapper.CurrentChar();
  return std::make_optional(TokenFactory::MakePunct(c, wrapper.GetLine(), wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
