#include "DefaultHandler.hpp"

#include "lib/lexer/LexerError.hpp"

namespace ovum::compiler::lexer {

OptToken DefaultHandler::Scan(SourceCodeWrapper& wrapper) {
  char c = wrapper.CurrentChar();
  throw LexerError(std::string("Unexpected character: ") + c);
}

} // namespace ovum::compiler::lexer
