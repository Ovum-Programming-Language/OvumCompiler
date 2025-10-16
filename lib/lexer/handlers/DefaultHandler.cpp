#include "DefaultHandler.hpp"

#include "lib/lexer/LexerError.hpp"

OptToken DefaultHandler::Scan(SourceCodeWrapper& wrapper) {
  char c = wrapper.CurrentChar();
  throw LexerError(std::string("Unexpected character: ") + c);
}
