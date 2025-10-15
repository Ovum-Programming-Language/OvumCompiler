#include "SlashHandler.hpp"

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

OptToken SlashHandler::Scan(Lexer &lx) {
  if (lx.Peek() == '/') {
    std::string txt;

    while (!lx.IsAtEnd() && lx.Peek() != '\n')
      txt.push_back(lx.Advance());

    if (lx.IsKeepComments())
      return std::make_optional(TokenFactory::make_comment(std::move(txt), lx.GetLine(), lx.GetTokenCol()));

    return std::nullopt;
  }

  if (lx.Peek() == '*') {
    lx.Advance();
    std::string txt;
    bool closed = false;

    while (!lx.IsAtEnd()) {
      char c = lx.Advance();

      if (c == '*' && lx.Peek() == '/') {
        lx.Advance();
        closed = true;
        break;
      }

      txt.push_back(c);
    }

    if (!closed)
      throw LexerError("Unterminated block comment");

    if (lx.IsKeepComments())
      return std::make_optional(TokenFactory::make_comment(std::move(txt), lx.GetLine(), lx.GetTokenCol()));

    return std::nullopt;
  }
  return std::make_optional(TokenFactory::make_operator(std::string(1, '/'), lx.GetLine(), lx.GetTokenCol()));
}
