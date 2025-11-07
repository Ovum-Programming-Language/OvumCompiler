#ifndef TOKENS_VALUE_HPP_
#define TOKENS_VALUE_HPP_

#include <memory>
#include <string>

namespace ovum {

class Value { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Value() = default;

  [[nodiscard]] virtual std::unique_ptr<Value> Clone() const = 0;

  [[nodiscard]] virtual std::string ToString() const = 0;

  [[nodiscard]] virtual std::string GetTypeName() const = 0;
};

} // namespace ovum

#endif // TOKENS_VALUE_HPP_
