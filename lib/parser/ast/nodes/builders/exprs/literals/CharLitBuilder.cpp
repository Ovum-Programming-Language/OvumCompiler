#include "CharLitBuilder.hpp"

namespace ovum::compiler::parser {

CharLitBuilder& CharLitBuilder::WithValue(char value) {
  node_->SetValue(value);
  return *this;
}

} // namespace ovum::compiler::parser
