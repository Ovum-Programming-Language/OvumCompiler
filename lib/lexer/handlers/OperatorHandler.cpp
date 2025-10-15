#include "OperatorHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken OperatorHandler::Scan(Lexer &lx) {
  std::string op;
  op.push_back(lx.CurrentChar());
  char p = lx.Peek();

  if (p != '\0') {
    std::string two = op + p;
    if (lx.IsMultiop(two)) {
      lx.Advance();
      op = two;
    }
  }

  return std::make_optional(TokenFactory::make_operator(std::move(op), lx.GetLine(), lx.GetTokenCol()));
}
