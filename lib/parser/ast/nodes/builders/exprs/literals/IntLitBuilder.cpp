#include "IntLitBuilder.hpp"

namespace ovum::compiler::parser {

IntLitBuilder& IntLitBuilder::WithValue(long long value) {
  node_->SetValue(value);
  return *this;
}

} // namespace ovum::compiler::parser
