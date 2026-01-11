#ifndef PARSER_IBINARYOPTAG_HPP_
#define PARSER_IBINARYOPTAG_HPP_

#include <string>

namespace ovum::compiler::parser {

class IBinaryOpTag { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~IBinaryOpTag() = default;

  [[nodiscard]] virtual std::string_view Name() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IBINARYOPTAG_HPP_
