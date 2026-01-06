#include "UnsafeBlock.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void UnsafeBlock::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Block* UnsafeBlock::Body() const noexcept {
  return body_.get();
}

Block* UnsafeBlock::MutableBody() noexcept {
  return body_.get();
}

void UnsafeBlock::SetBody(std::unique_ptr<Block> body_block) {
  body_ = std::move(body_block);
}

std::unique_ptr<Block> UnsafeBlock::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
