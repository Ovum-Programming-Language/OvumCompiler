#include "ByteLitBuilder.hpp"

namespace ovum::compiler::parser {

ByteLitBuilder& ByteLitBuilder::WithValue(uint8_t value) {
  node_->SetValue(value);
  return *this;
}

} // namespace ovum::compiler::parser

