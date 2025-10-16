#include "FloatValue.hpp"

#include <sstream>

FloatValue::FloatValue(long double x) : v(x) {
}

std::unique_ptr<Value> FloatValue::Clone() const {
  return std::make_unique<FloatValue>(v);
}

std::string FloatValue::ToString() const {
  std::ostringstream os;
  os << static_cast<double>(v);
  return os.str();
}

std::string FloatValue::GetTypeName() const {
  return "Float";
}
