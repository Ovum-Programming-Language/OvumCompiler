#ifndef PARSER_ITOKENSTREAM_HPP_
#define PARSER_ITOKENSTREAM_HPP_

#include <cstddef>

#include <tokens/Token.hpp>

namespace ovum::compiler::parser {

class ITokenStream {
public:
  virtual ~ITokenStream() = default;

  virtual const Token& Peek(size_t k = 0) = 0;
  virtual TokenPtr Consume() = 0;

  virtual size_t Position() const = 0;
  virtual void Rewind(size_t n) = 0;
  virtual bool IsEof() const = 0;

  virtual const Token* LastConsumed() const = 0;
  virtual const Token* TryPeek(size_t k = 0) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ITOKENSTREAM_HPP_
