#include "IdentifierHandler.hpp"

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken IdentifierHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string s;
  s.push_back(wrapper.CurrentChar());
  wrapper.ConsumeWhile(s, [](char ch) { return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'; });

  if (wrapper.IsKeyword(s)) {
    if (s == "true" || s == "false") {
      return std::make_optional(
          TokenFactory::MakeBoolLiteral(s, s == "true", wrapper.GetLine(), wrapper.GetTokenCol()));
    }

    return std::make_optional(TokenFactory::MakeKeyword(std::move(s), wrapper.GetLine(), wrapper.GetTokenCol()));
  }

  if (s == "xor") {
    return std::make_optional(TokenFactory::MakeOperator(std::move(s), wrapper.GetLine(), wrapper.GetTokenCol()));
  }

  return std::make_optional(TokenFactory::MakeIdent(std::move(s), wrapper.GetLine(), wrapper.GetTokenCol()));
}
