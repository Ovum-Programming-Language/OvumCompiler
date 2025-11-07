#ifndef TOKENS_FLOATVALUE_HPP_
#define TOKENS_FLOATVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

namespace ovum {

class FloatValue : public Value {
public:
  explicit FloatValue(double x);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  double v;
};

} // namespace ovum

#endif // TOKENS_FLOATVALUE_HPP_
