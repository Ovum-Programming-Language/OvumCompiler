#ifndef EOFTOKEN_HPP_
#define EOFTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class EofToken final : public Token {
public:
  EofToken(int32_t line, int32_t col);

  [[nodiscard]] TokenType GetType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] std::unique_ptr<Token> Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

private:
  std::string lexeme_;
};

#endif // EOFTOKEN_HPP_
