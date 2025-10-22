#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <string>

#include "TokenPosition.hpp"
#include "TokenVisitor.hpp"

class Token { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Token() = default;

  [[nodiscard]] virtual std::string GetStringType() const = 0;

  [[nodiscard]] virtual std::string GetLexeme() const = 0;

  [[nodiscard]] virtual std::shared_ptr<Token> Clone() const = 0;

  virtual void Accept(TokenVisitor& v) const = 0;

  [[nodiscard]] virtual std::string ToString() const = 0;

  [[nodiscard]] virtual const TokenPosition& GetPosition() const noexcept = 0;
};

using TokenPtr = std::shared_ptr<Token>;

#endif // TOKEN_HPP_
