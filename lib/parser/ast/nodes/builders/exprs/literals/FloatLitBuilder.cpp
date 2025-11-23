#include "FloatLitBuilder.hpp"

namespace ovum::compiler::parser {

FloatLitBuilder& FloatLitBuilder::WithValue(double value) {
  node_->SetValue(value);
  return *this;
}

} // namespace ovum::compiler::parser
