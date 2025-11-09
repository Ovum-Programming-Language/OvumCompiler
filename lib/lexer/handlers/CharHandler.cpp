#include "CharHandler.hpp"

#include <tokens/TokenFactory.hpp>

#include "lib/lexer/LexerError.hpp"

namespace ovum::compiler::lexer {

std::expected<OptToken, LexerError> CharHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string raw;
  raw.push_back('\'');
  char val = '\0';

  if (wrapper.Peek() == '\\') {
    wrapper.Advance();
    raw.push_back('\\');
    char e = wrapper.Advance();
    raw.push_back(e);
    switch (e) {
      case 'n':
        val = '\n';
        break;
      case 't':
        val = '\t';
        break;
      case '\\':
        val = '\\';
        break;
      case '\'':
        val = '\'';
        break;
      case '0':
        val = '\0';
        break;
      default:
        return std::unexpected(LexerError(std::string("Unknown escape in char literal: \\") + e));
    }
  } else {
    char c = wrapper.Advance();
    raw.push_back(c);
    val = c;
  }

  if (wrapper.IsAtEnd()) {
    return std::unexpected(LexerError("Unterminated char literal"));
  }

  if (wrapper.Peek() == '\n') {
    return std::unexpected(LexerError("Newline in char literal"));
  }

  if (wrapper.Peek() == '\'') {
    wrapper.Advance();
    raw.push_back('\'');
  } else {
    return std::unexpected(LexerError("Too many characters in char literal"));
  }

  return std::make_optional(
      TokenFactory::MakeCharLiteral(std::move(raw), val, wrapper.GetLine(), wrapper.GetTokenCol()));
}

} // namespace ovum::compiler::lexer
