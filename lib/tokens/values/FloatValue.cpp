#include "FloatValue.hpp"

#include <sstream>

namespace ovum {

FloatValue::FloatValue(double x) : v(x) {
}

std::unique_ptr<Value> FloatValue::Clone() const {
  return std::make_unique<FloatValue>(v);
}

std::string FloatValue::ToString() const {
  std::ostringstream os;
  os << v;
  return os.str();
}

std::string FloatValue::GetTypeName() const {
  return "Float";
}

} // namespace ovum
