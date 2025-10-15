#include "DefaultHandler.hpp"

OptToken DefaultHandler::Scan(Lexer &lx) {
  char c = lx.CurrentChar();
  throw LexerError(std::string("Unexpected character: ") + c);
}
