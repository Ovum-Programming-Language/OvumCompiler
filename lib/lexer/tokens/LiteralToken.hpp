#ifndef LITERALTOKEN_HPP_
#define LITERALTOKEN_HPP_

#include <memory>
#include <string>
#include "Token.hpp"
#include "TokenType.hpp"
#include "TokenVisitor.hpp"
#include "lib/lexer/values/Value.hpp"

class LiteralToken final : public Token {
public:
  LiteralToken(TokenType typ, std::string rawLexeme, std::unique_ptr<Value> value, int32_t line, int32_t col);

  [[nodiscard]] TokenType GetType() const noexcept override;
  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] const Value* GetValue() const noexcept;
  Value* GetValue() noexcept;

  [[nodiscard]] std::unique_ptr<Token> Clone() const override;
  void Accept(TokenVisitor& visitor) const override;
  [[nodiscard]] std::string ToString() const override;

private:
  TokenType typ_;
  std::string lexeme_;
  std::unique_ptr<Value> value_;
};

#endif // LITERALTOKEN_HPP_
