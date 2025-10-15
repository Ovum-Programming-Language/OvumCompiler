#include "StringHandler.hpp"

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

OptToken StringHandler::Scan(Lexer &lx) {
  std::string raw;
  std::string out;
  raw.push_back('"');

  while (!lx.IsAtEnd()) {
    char c = lx.Advance();
    raw.push_back(c);

    if (c == '"') {
      return std::make_optional(
          TokenFactory::make_string_literal(std::move(raw), std::move(out), lx.GetLine(), lx.GetTokenCol()));
    }

    if (c == '\\') {
      if (lx.IsAtEnd())
        throw LexerError("Unterminated string literal (backslash at EOF)");
      char e = lx.Advance();
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
        default:
          out.push_back(e);
          break;
      }
    } else {
      if (c == '\n')
        throw LexerError("Unterminated string literal (newline inside)");

      out.push_back(c);
    }
  }

  throw LexerError("Unterminated string literal (EOF reached)");
}
