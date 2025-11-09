#ifndef PARSER_IASSIGNOPTAG_HPP_
#define PARSER_IASSIGNOPTAG_HPP_

#include <string>

namespace ovum::compiler::parser {

class IAssignOpTag {
public:
  virtual ~IAssignOpTag() = default;

  virtual std::string_view Name() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IASSIGNOPTAG_HPP_
