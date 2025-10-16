#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <string>

#include "TokenVisitor.hpp"
#include "TokenPosition.hpp"

class Token {
public:
  virtual ~Token() = default;

  [[nodiscard]] virtual std::string GetStringType() const = 0;

  [[nodiscard]] virtual std::string GetLexeme() const = 0;

  [[nodiscard]] virtual std::unique_ptr<Token> Clone() const = 0;

  virtual void Accept(TokenVisitor& v) const = 0;

  [[nodiscard]] virtual std::string ToString() const = 0;

  [[nodiscard]] virtual const TokenPosition& GetPosition() const noexcept = 0;
};

#endif // TOKEN_HPP_
