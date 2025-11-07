#include "WhitespaceHandler.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> WhitespaceHandler::Scan(SourceCodeWrapper& wrapper) {
  while (!wrapper.IsAtEnd()) {
    if (char p = wrapper.Peek(); p != ' ' && p != '\t' && p != '\r') {
      break;
    }

    wrapper.Advance();
  }

  return std::nullopt;
}

} // namespace ovum::compiler::lexer
