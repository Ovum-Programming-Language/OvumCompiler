#ifndef CHARVALUE_HPP_
#define CHARVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

class CharValue : public Value {
public:
  explicit CharValue(char c);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;
  [[nodiscard]] std::string ToString() const override;
  [[nodiscard]] std::string GetTypeName() const override;

private:
  char v;
};

#endif // CHARVALUE_HPP_
