#include "ColonHandler.hpp"

#include <tokens/TokenFactory.hpp>

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> ColonHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string op;
  op.push_back(wrapper.CurrentChar());

  char next = wrapper.Peek();

  if (next != '\0') {
    std::string two = op + next;
    if (SourceCodeWrapper::IsMultiOp(two)) {
      wrapper.Advance();
      op = two;
      return std::make_optional(TokenFactory::MakeOperator(std::move(op), wrapper.GetLine(), wrapper.GetTokenCol()));
    }
  }

  return std::make_optional(TokenFactory::MakePunct(':', wrapper.GetLine(), wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
