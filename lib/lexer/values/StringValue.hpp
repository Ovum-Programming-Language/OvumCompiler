#ifndef STRINGVALUE_HPP_
#define STRINGVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

class StringValue : public Value {
public:
  explicit StringValue(std::string s);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  std::string v;
};

#endif // STRINGVALUE_HPP_
