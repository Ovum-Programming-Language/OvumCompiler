#ifndef FLOATVALUE_HPP_
#define FLOATVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

class FloatValue : public Value {
public:
  explicit FloatValue(double x);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  double v;
};

#endif // FLOATVALUE_HPP_
