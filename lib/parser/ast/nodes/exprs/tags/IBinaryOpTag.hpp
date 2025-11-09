#ifndef PARSER_IBINARYOPTAG_HPP_
#define PARSER_IBINARYOPTAG_HPP_

#include <string>

namespace ovum::compiler::parser {

class IBinaryOpTag {
public:
  virtual ~IBinaryOpTag() = default;

  virtual std::string_view Name() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IBINARYOPTAG_HPP_
