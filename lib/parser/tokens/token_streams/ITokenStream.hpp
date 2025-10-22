#ifndef ITOKENSTREAM_HPP_
#define ITOKENSTREAM_HPP_

#include <cstddef>

#include "lib/lexer/tokens/Token.hpp"

class ITokenStream {
public:
  virtual ~ITokenStream() = default;

  virtual const Token& Peek(std::size_t k = 0) = 0;

  virtual TokenPtr Consume() = 0;

  virtual std::size_t Position() const = 0;

  virtual void Rewind(std::size_t n) = 0;

  virtual bool IsEof() const = 0;

  virtual const Token* LastConsumed() const = 0;

  virtual const Token* TryPeek(std::size_t k = 0) = 0;
};

#endif // ITOKENSTREAM_HPP_
