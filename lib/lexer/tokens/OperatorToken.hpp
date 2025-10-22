#ifndef OPERATORTOKEN_HPP_
#define OPERATORTOKEN_HPP_

#include <string>

#include "Token.hpp"

class TokenVisitor;

class OperatorToken final : public Token {
public:
  OperatorToken(std::string op, const TokenPosition& position);

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

#endif // OPERATORTOKEN_HPP_
