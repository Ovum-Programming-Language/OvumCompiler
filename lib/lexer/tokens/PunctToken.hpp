#ifndef PUNCTTOKEN_HPP_
#define PUNCTTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class PunctToken final : public Token {
public:
  PunctToken(char ch, int32_t line, int32_t col);
  PunctToken(std::string punct, int32_t line, int32_t col);

  [[nodiscard]] TokenType GetType() const noexcept override;
  [[nodiscard]] std::string GetLexeme() const noexcept override;
  [[nodiscard]] std::unique_ptr<Token> Clone() const override;
  void Accept(TokenVisitor& visitor) const override;
  [[nodiscard]] std::string ToString() const override;

private:
  std::string lexeme_;
};

#endif // PUNCTTOKEN_HPP_
