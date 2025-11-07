#ifndef TOKENS_INTVALUE_HPP_
#define TOKENS_INTVALUE_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "Value.hpp"

namespace ovum {

class IntValue : public Value {
public:
  explicit IntValue(int64_t x);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  int64_t v;
};

} // namespace ovum

#endif // TOKENS_INTVALUE_HPP_
