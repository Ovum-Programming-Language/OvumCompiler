#include "DefaultHandler.hpp"

#include "lib/lexer/LexerError.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> DefaultHandler::Scan(SourceCodeWrapper& wrapper) {
  char c = wrapper.CurrentChar();
  return std::unexpected(LexerError(std::string("Unexpected character: ") + c));
}

} // namespace ovum::compiler::lexer
