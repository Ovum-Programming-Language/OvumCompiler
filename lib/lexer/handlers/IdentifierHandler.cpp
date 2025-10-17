#include "IdentifierHandler.hpp"

#include <limits>
#include <cmath>

#include "lib/lexer/tokens/TokenFactory.hpp"

OptToken IdentifierHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string s;
  s.push_back(wrapper.CurrentChar());
  wrapper.ConsumeWhile(s,
                       [](char ch) {
                         return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
                       });

  if (s == "Inf" || s == "inf" || s == "Infinity" || s == "infinity") {
    return std::make_optional(
        TokenFactory::MakeFloatLiteral(s,
                                       std::numeric_limits<long double>::infinity(),
                                       wrapper.GetLine(),
                                       wrapper.GetTokenCol()));
  }

  if (s == "NaN") {
    return std::make_optional(
        TokenFactory::MakeFloatLiteral(s, std::nanl(""), wrapper.GetLine(), wrapper.GetTokenCol()));
  }

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
