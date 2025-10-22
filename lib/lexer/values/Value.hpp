#ifndef VALUE_HPP_
#define VALUE_HPP_

#include <memory>
#include <string>

class Value { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Value() = default;

  [[nodiscard]] virtual std::unique_ptr<Value> Clone() const = 0;

  [[nodiscard]] virtual std::string ToString() const = 0;

  [[nodiscard]] virtual std::string GetTypeName() const = 0;
};

#endif // VALUE_HPP_
