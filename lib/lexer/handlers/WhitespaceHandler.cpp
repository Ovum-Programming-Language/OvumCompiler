#include "WhitespaceHandler.hpp"

OptToken WhitespaceHandler::Scan(SourceCodeWrapper& wrapper) {
  while (!wrapper.IsAtEnd()) {
    if (char p = wrapper.Peek(); p == ' ' || p == '\t' || p == '\r') {
      wrapper.Advance();
      continue;
    }

    break;
  }
  return std::nullopt;
}
