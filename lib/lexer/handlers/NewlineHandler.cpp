#include "NewlineHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken NewlineHandler::Scan(Lexer &lx) {
  return std::make_optional(TokenFactory::make_newline(lx.GetLine(), lx.GetTokenCol()));
}
