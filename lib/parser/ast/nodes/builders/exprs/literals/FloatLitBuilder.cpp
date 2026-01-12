#include "FloatLitBuilder.hpp"

namespace ovum::compiler::parser {

FloatLitBuilder& FloatLitBuilder::WithValue(double value) {
  node_->SetValue(value);
  return *this;
}

FloatLitBuilder& FloatLitBuilder::WithValue(long double value) {
  node_->SetValue(static_cast<double>(value));
  return *this;
}

} // namespace ovum::compiler::parser
