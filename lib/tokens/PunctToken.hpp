#ifndef TOKENS_PUNCTTOKEN_HPP_
#define TOKENS_PUNCTTOKEN_HPP_

#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

namespace ovum {

class PunctToken final : public Token {
public:
  PunctToken(char ch, const TokenPosition& position);

  PunctToken(std::string punct, const TokenPosition& position);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] TokenPtr Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const TokenPosition& GetPosition() const noexcept override;

private:
  std::string lexeme_;
  TokenPosition position_;
};

} // namespace ovum

#endif // TOKENS_PUNCTTOKEN_HPP_
