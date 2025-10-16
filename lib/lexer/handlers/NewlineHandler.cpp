#include "NewlineHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken NewlineHandler::Scan(SourceCodeWrapper& wrapper) {
  return std::make_optional(TokenFactory::MakeNewline(wrapper.GetLine(), wrapper.GetTokenCol()));
}
