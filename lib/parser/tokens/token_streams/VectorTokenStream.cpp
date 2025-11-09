#include "lib/parser/tokens/token_streams/VectorTokenStream.hpp"

#include <stdexcept>
#include <utility>

VectorTokenStream::VectorTokenStream(std::vector<TokenPtr> tokens) : tokens_(std::move(tokens)) {
}

const Token& VectorTokenStream::Peek(size_t k) {
  const Token* token = TryPeek(k);

  if (token != nullptr) {
    return *token;
  }

  if (last_ != nullptr) {
    return *last_;
  }

  if (!tokens_.empty()) {
    return *tokens_.back();
  }

  throw std::out_of_range("VectorTokenStream::Peek out of range");
}

TokenPtr VectorTokenStream::Consume() {
  if (index_ < tokens_.size()) {
    last_ = tokens_[index_].get();
    return std::move(tokens_[index_++]);
  }

  last_ = nullptr;
  return nullptr;
}

size_t VectorTokenStream::Position() const {
  return index_;
}

void VectorTokenStream::Rewind(size_t n) {
  if (n > index_) {
    index_ = 0;
  } else {
    index_ -= n;
  }

  last_ = nullptr;
}

bool VectorTokenStream::IsEof() const {
  return index_ >= tokens_.size();
}

const Token* VectorTokenStream::LastConsumed() const {
  return last_;
}

const Token* VectorTokenStream::TryPeek(size_t k) {
  const size_t pos = index_ + k;
  if (pos < tokens_.size()) {
    return tokens_[pos].get();
  }

  return nullptr;
}

size_t VectorTokenStream::Size() const {
  return tokens_.size();
}
