#include "IntValue.hpp"

namespace ovum {

IntValue::IntValue(int64_t x) : v(x) {
}

std::unique_ptr<Value> IntValue::Clone() const {
  return std::make_unique<IntValue>(v);
}

std::string IntValue::ToString() const {
  return std::to_string(v);
}

std::string IntValue::GetTypeName() const {
  return "Int";
}

} // namespace ovum
