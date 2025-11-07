#ifndef TOKENS_STRINGVALUE_HPP_
#define TOKENS_STRINGVALUE_HPP_

#include <memory>
#include <string>

#include "Value.hpp"

namespace ovum {

class StringValue : public Value {
public:
  explicit StringValue(std::string s);

  [[nodiscard]] std::unique_ptr<Value> Clone() const override;

  [[nodiscard]] std::string ToString() const override;

  [[nodiscard]] std::string GetTypeName() const override;

private:
  std::string v;
};

} // namespace ovum

#endif // TOKENS_STRINGVALUE_HPP_
