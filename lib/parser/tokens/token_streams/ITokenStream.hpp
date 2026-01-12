#ifndef PARSER_ITOKENSTREAM_HPP_
#define PARSER_ITOKENSTREAM_HPP_

#include <tokens/Token.hpp>

namespace ovum::compiler::parser {

class ITokenStream { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~ITokenStream() = default;

  virtual const Token& Peek(size_t k = 0) = 0;
  virtual TokenPtr Consume() = 0;

  [[nodiscard]] virtual size_t Position() const = 0;
  virtual void Rewind(size_t n) = 0;
  [[nodiscard]] virtual bool IsEof() const = 0;

  [[nodiscard]] virtual const Token* LastConsumed() const = 0;
  virtual const Token* TryPeek(size_t k = 0) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ITOKENSTREAM_HPP_
