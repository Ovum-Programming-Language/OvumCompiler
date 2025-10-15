#include "NewlineHandler.hpp"

OptToken NewlineHandler::Scan(Lexer &lx) {
  return std::make_optional(TokenFactory::make_newline(lx.GetLine(), lx.GetTokenCol()));
}
