#ifndef TOKENS_COMMENTTOKEN_HPP_
#define TOKENS_COMMENTTOKEN_HPP_

#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

namespace ovum {

class CommentToken final : public Token {
public:
  CommentToken(std::string txt, const TokenPosition& position);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] TokenPtr Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const TokenPosition& GetPosition() const noexcept override;

private:
  std::string text_;
  TokenPosition position_;
};

} // namespace ovum

#endif // TOKENS_COMMENTTOKEN_HPP_
