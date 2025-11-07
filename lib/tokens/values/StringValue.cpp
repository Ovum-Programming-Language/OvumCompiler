#include "StringValue.hpp"

#include <utility>

namespace ovum {

StringValue::StringValue(std::string s) : v(std::move(s)) {
}

std::unique_ptr<Value> StringValue::Clone() const {
  return std::make_unique<StringValue>(v);
}

std::string StringValue::ToString() const {
  return "\"" + v + "\"";
}

std::string StringValue::GetTypeName() const {
  return "String";
}

} // namespace ovum
