#include "OperatorHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken OperatorHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string op;
  op.push_back(wrapper.CurrentChar());
  char p = wrapper.Peek();

  if (p != '\0') {
    std::string two = op + p;

    if (wrapper.IsMultiop(two)) {
      wrapper.Advance();
      op = two;
    }
  }

  return std::make_optional(TokenFactory::MakeOperator(std::move(op), wrapper.GetLine(), wrapper.GetTokenCol()));
}
