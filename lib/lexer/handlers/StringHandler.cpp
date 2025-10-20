#include "StringHandler.hpp"

#include <utility>

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

OptToken StringHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string raw;
  std::string out;
  raw.push_back('"');

  while (!wrapper.IsAtEnd()) {
    char c = wrapper.Advance();
    raw.push_back(c);

    if (c == '"') {
      return std::make_optional(
          TokenFactory::MakeStringLiteral(std::move(raw), std::move(out), wrapper.GetLine(), wrapper.GetTokenCol()));
    }

    if (c == '\\') {
      if (wrapper.IsAtEnd()) {
        throw LexerError("Unterminated string literal (backslash at EOF)");
      }

      char e = wrapper.Advance();
      raw.push_back(e);

      switch (e) {
        case 'n':
          out.push_back('\n');
          break;
        case 't':
          out.push_back('\t');
          break;
        case 'r':
          out.push_back('\r');
          break;
        case '\\':
          out.push_back('\\');
          break;
        case '"':
          out.push_back('"');
          break;
        case '0':
          out.push_back('\0');
          break;
        default:
          throw LexerError(std::string("Unknown escape in string literal: \\") + e);
      }
    } else {
      if (c == '\n') {
        throw LexerError("Unterminated string literal (newline inside)");
      }

      out.push_back(c);
    }
  }

  throw LexerError("Unterminated string literal (EOF reached)");
}
