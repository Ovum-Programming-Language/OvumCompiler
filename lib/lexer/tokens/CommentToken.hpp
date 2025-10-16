#ifndef COMMENTTOKEN_HPP_
#define COMMENTTOKEN_HPP_

#include <memory>
#include <string>

#include "Token.hpp"
#include "TokenVisitor.hpp"

class CommentToken final : public Token {
public:
  CommentToken(std::string txt, const TokenPosition& position);

  [[nodiscard]] std::string GetStringType() const noexcept override;

  [[nodiscard]] std::string GetLexeme() const noexcept override;

  [[nodiscard]] std::unique_ptr<Token> Clone() const override;

  void Accept(TokenVisitor& visitor) const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] const TokenPosition& GetPosition() const noexcept override;

private:
  std::string text_;
  TokenPosition position_;
};

#endif // COMMENTTOKEN_HPP_
