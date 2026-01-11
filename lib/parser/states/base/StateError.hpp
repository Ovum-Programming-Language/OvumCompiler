#ifndef PARSER_STATEERROR_HPP_
#define PARSER_STATEERROR_HPP_

#include <string>
#include <string_view>

namespace ovum::compiler::parser {

class StateError {
public:
  StateError();
  explicit StateError(std::string message);
  explicit StateError(std::string_view message);

  [[nodiscard]] const std::string& Message() const noexcept;

private:
  std::string message_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATEERROR_HPP_
