#include "CharHandler.hpp"

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

OptToken CharHandler::Scan(Lexer& lx) {
  std::string raw;
  raw.push_back('\'');
  char val = '\0';

  if (lx.Peek() == '\\') {
    lx.Advance();
    raw.push_back('\\');
    char e = lx.Advance();
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
      default:
        val = e;
        break;
    }
  } else {
    char c = lx.Advance();
    raw.push_back(c);
    val = c;
  }

  if (lx.Peek() == '\'') {
    lx.Advance();
    raw.push_back('\'');
  } else {
    throw LexerError("Unterminated char literal");
  }

  return std::make_optional(
      TokenFactory::MakeCharLiteral(std::move(raw),
                                    val,
                                    lx.GetLine(),
                                    lx.GetTokenCol()));
}
