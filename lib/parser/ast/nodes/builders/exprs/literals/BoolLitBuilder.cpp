#include "BoolLitBuilder.hpp"

namespace ovum::compiler::parser {

BoolLitBuilder& BoolLitBuilder::WithValue(bool value) {
  node_->SetValue(value);
  return *this;
}

} // namespace ovum::compiler::parser
