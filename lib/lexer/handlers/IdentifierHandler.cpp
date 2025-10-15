#include "IdentifierHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken IdentifierHandler::Scan(Lexer &lx) {
  std::string s;
  s.push_back(lx.CurrentChar());
  lx.ConsumeWhile(s, [](char ch) { return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'; });

  if (lx.IsKeyword(s)) {
    if (s == "true" || s == "false") {
      return std::make_optional(TokenFactory::make_bool_literal(s, s == "true", lx.GetLine(), lx.GetTokenCol()));
    }

    return std::make_optional(TokenFactory::make_keyword(std::move(s), lx.GetLine(), lx.GetTokenCol()));
  }

  if (s == "xor") {
    return std::make_optional(TokenFactory::make_operator(std::move(s), lx.GetLine(), lx.GetTokenCol()));
  }

  return std::make_optional(TokenFactory::make_ident(std::move(s), lx.GetLine(), lx.GetTokenCol()));
}
