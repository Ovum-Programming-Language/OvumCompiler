#ifndef BOOLVALUE_HPP_
#define BOOLVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

class BoolValue : public Value {
public:
  explicit BoolValue(bool b);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  bool v;
};

#endif // BOOLVALUE_HPP_
