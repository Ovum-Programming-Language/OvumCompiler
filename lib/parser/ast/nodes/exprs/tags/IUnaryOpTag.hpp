#ifndef PARSER_IUNARYOPTAG_HPP_
#define PARSER_IUNARYOPTAG_HPP_

#include <string>

namespace ovum::compiler::parser {

class IUnaryOpTag { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~IUnaryOpTag() = default;

  [[nodiscard]] virtual std::string_view Name() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IUNARYOPTAG_HPP_
