#include "SlashHandler.hpp"

#include <tokens/TokenFactory.hpp>

#include "lib/lexer/LexerError.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> SlashHandler::Scan(SourceCodeWrapper& wrapper) {
  if (wrapper.Peek() == '/') {
    std::string txt;

    while (!wrapper.IsAtEnd() && wrapper.Peek() != '\n') {
      txt.push_back(wrapper.Advance());
    }

    if (wrapper.IsKeepComments()) {
      std::string comment = txt.substr(1, txt.size() - 1);
      return std::make_optional(
          TokenFactory::MakeComment(std::move(comment), wrapper.GetLine(), wrapper.GetTokenCol()));
    }

    return std::nullopt;
  }

  if (wrapper.Peek() == '*') {
    wrapper.Advance();
    std::string txt;
    bool closed = false;

    while (!wrapper.IsAtEnd()) {
      char c = wrapper.Advance();

      if (c == '*' && wrapper.Peek() == '/') {
        wrapper.Advance();
        closed = true;
        break;
      }

      txt.push_back(c);
    }

    if (!closed) {
      return std::unexpected(LexerError("Unterminated block comment"));
    }

    if (wrapper.IsKeepComments()) {
      return std::make_optional(TokenFactory::MakeComment(std::move(txt), wrapper.GetLine(), wrapper.GetTokenCol()));
    }

    return std::nullopt;
  }

  return std::make_optional(TokenFactory::MakeOperator(std::string(1, '/'), wrapper.GetLine(), wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
