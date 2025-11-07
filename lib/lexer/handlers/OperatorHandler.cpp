#include "OperatorHandler.hpp"

#include "lib/tokens/TokenFactory.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> OperatorHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string op;
  op.push_back(wrapper.CurrentChar());
  char p = wrapper.Peek();

  if (p != '\0') {
    std::string two = op + p;

    if (SourceCodeWrapper::IsMultiOp(two)) {
      wrapper.Advance();
      op = two;
    }
  }

  return std::make_optional(TokenFactory::MakeOperator(std::move(op), wrapper.GetLine(), wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
