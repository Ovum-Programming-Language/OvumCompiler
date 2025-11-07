#ifndef TOKENS_EOFTOKEN_HPP_
#define TOKENS_EOFTOKEN_HPP_

#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

namespace ovum {

class EofToken final : public Token {
public:
  EofToken(const TokenPosition& position);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] TokenPtr Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const TokenPosition& GetPosition() const noexcept override;

private:
  TokenPosition position_;
};

} // namespace ovum

#endif // TOKENS_EOFTOKEN_HPP_
