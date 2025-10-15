#include "IdentifierHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken IdentifierHandler::Scan(Lexer& lx) {
  std::string s;
  s.push_back(lx.CurrentChar());
  lx.ConsumeWhile(s,
                  [](char ch) {
                    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
                  });

  if (lx.IsKeyword(s)) {
    if (s == "true" || s == "false") {
      return std::make_optional(
          TokenFactory::MakeBoolLiteral(s,
                                          s == "true",
                                          lx.GetLine(),
                                          lx.GetTokenCol()));
    }

    return std::make_optional(
        TokenFactory::MakeKeyword(std::move(s),
                                   lx.GetLine(),
                                   lx.GetTokenCol()));
  }

  if (s == "xor") {
    return std::make_optional(
        TokenFactory::MakeOperator(std::move(s),
                                    lx.GetLine(),
                                    lx.GetTokenCol()));
  }

  return std::make_optional(
      TokenFactory::MakeIdent(std::move(s), lx.GetLine(), lx.GetTokenCol()));
}
