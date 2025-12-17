#include "UnsafeBlockBuilder.hpp"

namespace ovum::compiler::parser {

UnsafeBlockBuilder& UnsafeBlockBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser