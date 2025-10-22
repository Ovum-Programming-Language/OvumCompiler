#ifndef INTVALUE_HPP_
#define INTVALUE_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "Value.hpp"

class IntValue : public Value {
public:
  explicit IntValue(int64_t x);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  int64_t v;
};

#endif // INTVALUE_HPP_
