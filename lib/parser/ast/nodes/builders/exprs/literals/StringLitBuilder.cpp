#include "StringLitBuilder.hpp"

namespace ovum::compiler::parser {

StringLitBuilder& StringLitBuilder::WithValue(std::string value) {
  node_->SetValue(std::move(value));
  return *this;
}

} // namespace ovum::compiler::parser
