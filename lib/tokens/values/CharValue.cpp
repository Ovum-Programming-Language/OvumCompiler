#include "CharValue.hpp"

namespace ovum {

CharValue::CharValue(char c) : v(c) {
}

std::unique_ptr<Value> CharValue::Clone() const {
  return std::make_unique<CharValue>(v);
}

std::string CharValue::ToString() const {
  return "'" + std::string(1, v) + "'";
}

std::string CharValue::GetTypeName() const {
  return "Char";
}

} // namespace ovum
