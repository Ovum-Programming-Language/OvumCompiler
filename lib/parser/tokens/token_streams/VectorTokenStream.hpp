#ifndef VECTORTOKENSTREAM_HPP_
#define VECTORTOKENSTREAM_HPP_

#include <cstddef>
#include <vector>

#include "ITokenStream.hpp"
#include "lib/lexer/tokens/Token.hpp"

class VectorTokenStream : public ITokenStream {
public:
  explicit VectorTokenStream(std::vector<TokenPtr> tokens);

  const Token& Peek(std::size_t k = 0) override;

  TokenPtr Consume() override;

  std::size_t Position() const override;

  void Rewind(std::size_t n) override;

  bool IsEof() const override;

  const Token* LastConsumed() const override;

  const Token* TryPeek(std::size_t k = 0) override;

  std::size_t Size() const {
    return tokens_.size();
  }

private:
  std::vector<TokenPtr> tokens_;
  std::size_t index_ = 0;
  const Token* last_ = nullptr;
};

#endif // VECTORTOKENSTREAM_HPP_
