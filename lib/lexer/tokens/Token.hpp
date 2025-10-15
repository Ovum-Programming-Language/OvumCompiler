#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <memory>
#include <sstream>
#include <string>

#include "TokenType.hpp"
#include "TokenVisitor.hpp"

class Token {
public:
  Token(int32_t line, int32_t column) : line_(line), column_(column) {
  }
  virtual ~Token() = default;
  [[nodiscard]] virtual TokenType GetType() const = 0;
  [[nodiscard]] virtual std::string GetLexeme() const = 0;
  [[nodiscard]] virtual std::unique_ptr<Token> Clone() const = 0;
  virtual void Accept(TokenVisitor& v) const = 0;
  [[nodiscard]] virtual std::string ToString() const {
    std::ostringstream os;
    os << "Token(" << to_string_view(GetType()) << ", '" << GetLexeme() << "', @" << line_ << ":" << column_ << ")";
    return os.str();
  }
  [[nodiscard]] int32_t GetLine() const noexcept {
    return line_;
  }
  [[nodiscard]] int32_t GetColumn() const noexcept {
    return column_;
  }

private:
  int32_t line_;
  int32_t column_;
};

#endif // TOKEN_HPP_
