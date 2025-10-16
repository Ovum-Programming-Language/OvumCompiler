#include "PunctHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken PunctHandler::Scan(SourceCodeWrapper& wrapper) {
  char c = wrapper.CurrentChar();
  return std::make_optional(TokenFactory::MakePunct(c, wrapper.GetLine(), wrapper.GetTokenCol()));
}
