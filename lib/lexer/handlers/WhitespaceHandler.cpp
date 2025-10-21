#include "WhitespaceHandler.hpp"

OptToken WhitespaceHandler::Scan(SourceCodeWrapper& wrapper) {
  while (!wrapper.IsAtEnd()) {
    if (char p = wrapper.Peek(); p != ' ' && p != '\t' && p != '\r') {
      break;
    }

    wrapper.Advance();
  }

  return std::nullopt;
}
