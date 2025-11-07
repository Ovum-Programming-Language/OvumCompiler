#ifndef TOKENS_CHARVALUE_HPP_
#define TOKENS_CHARVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

namespace ovum {

class CharValue : public Value {
public:
  explicit CharValue(char c);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  char v;
};

} // namespace ovum

#endif // TOKENS_CHARVALUE_HPP_
