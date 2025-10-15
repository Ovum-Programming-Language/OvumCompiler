#include "DefaultHandler.hpp"

#include "lib/lexer/LexerError.hpp"

OptToken DefaultHandler::Scan(Lexer& lx) {
  char c = lx.CurrentChar();
  throw LexerError(std::string("Unexpected character: ") + c);
}
