#include "PunctHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken PunctHandler::Scan(Lexer& lx) {
  char c = lx.CurrentChar();
  return std::make_optional(
      TokenFactory::MakePunct(c, lx.GetLine(), lx.GetTokenCol()));
}
