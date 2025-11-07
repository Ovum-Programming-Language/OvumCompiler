#include "BoolValue.hpp"

namespace ovum {

BoolValue::BoolValue(bool b) : v(b) {
}

std::unique_ptr<Value> BoolValue::Clone() const {
  return std::make_unique<BoolValue>(v);
}

std::string BoolValue::ToString() const {
  return v ? "true" : "false";
}

std::string BoolValue::GetTypeName() const {
  return "Bool";
}

} // namespace ovum
