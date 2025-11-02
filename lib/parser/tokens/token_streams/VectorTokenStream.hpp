#ifndef VECTORTOKENSTREAM_HPP_
#define VECTORTOKENSTREAM_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include "ITokenStream.hpp"
#include "lib/lexer/tokens/Token.hpp"

class VectorTokenStream : public ITokenStream {
public:
  explicit VectorTokenStream(std::vector<TokenPtr> tokens);

  const Token& Peek(size_t k = 0) override;

  TokenPtr Consume() override;

  size_t Position() const override;

  void Rewind(size_t n) override;

  bool IsEof() const override;

  const Token* LastConsumed() const override;

  const Token* TryPeek(size_t k = 0) override;

  size_t Size() const;

private:
  std::vector<TokenPtr> tokens_;
  size_t index_ = 0;
  const Token* last_ = nullptr;
};

#endif // VECTORTOKENSTREAM_HPP_
