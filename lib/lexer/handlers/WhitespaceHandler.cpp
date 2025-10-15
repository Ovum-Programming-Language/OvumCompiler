#include "WhitespaceHandler.hpp"

OptToken WhitespaceHandler::Scan(Lexer &lx) {
  while (!lx.IsAtEnd()) {
    if (char p = lx.Peek(); p == ' ' || p == '\t' || p == '\r') {
      lx.Advance();
      continue;
    }

    break;
  }
  return std::nullopt;
}
