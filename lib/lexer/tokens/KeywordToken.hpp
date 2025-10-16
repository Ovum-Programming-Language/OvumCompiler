#ifndef KEYWORDTOKEN_HPP_
#define KEYWORDTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class KeywordToken final : public Token {
public:
  KeywordToken(std::string lex, int32_t line, int32_t col);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] std::unique_ptr<Token> Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

private:
  std::string lexeme_;
};

#endif // KEYWORDTOKEN_HPP_
