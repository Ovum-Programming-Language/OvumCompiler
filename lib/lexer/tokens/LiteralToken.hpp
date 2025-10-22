#ifndef LITERALTOKEN_HPP_
#define LITERALTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"
#include "lib/lexer/values/Value.hpp"

class LiteralToken final : public Token {
public:
  LiteralToken(std::string rawLexeme, std::unique_ptr<Value> value, const TokenPosition& position);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] const Value* GetValue() const noexcept;

  Value* GetValue() noexcept;

  [[nodiscard]] TokenPtr Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const TokenPosition& GetPosition() const noexcept override;

private:
  std::string lexeme_;
  std::unique_ptr<Value> value_;
  TokenPosition position_;
};

#endif // LITERALTOKEN_HPP_
