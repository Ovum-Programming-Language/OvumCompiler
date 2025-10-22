#ifndef NEWLINETOKEN_HPP_
#define NEWLINETOKEN_HPP_

#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class NewlineToken final : public Token {
public:
  NewlineToken(const TokenPosition& position);

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

#endif // NEWLINETOKEN_HPP_
