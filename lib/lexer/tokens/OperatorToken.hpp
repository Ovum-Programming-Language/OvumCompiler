#ifndef OPERATORTOKEN_HPP_
#define OPERATORTOKEN_HPP_

#include <memory>
#include <string>
#include "Token.hpp"
#include "TokenType.hpp"

class TokenVisitor;

class OperatorToken final : public Token {
public:
  OperatorToken(std::string op, int32_t line, int32_t col);

  [[nodiscard]] TokenType GetType() const noexcept override;
  [[nodiscard]] std::string GetLexeme() const noexcept override;
  [[nodiscard]] std::unique_ptr<Token> Clone() const override;
  void Accept(TokenVisitor& visitor) const override;
  [[nodiscard]] std::string ToString() const override;

private:
  std::string lexeme_;
};

#endif // OPERATORTOKEN_HPP_
