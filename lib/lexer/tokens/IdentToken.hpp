#ifndef IDENTTOKEN_HPP_
#define IDENTTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class IdentToken final : public Token {
public:
  IdentToken(std::string lex, int32_t line, int32_t col);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] std::unique_ptr<Token> Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const std::string& GetName() const noexcept;

private:
  std::string lexeme_;
};

#endif // IDENTTOKEN_HPP_
