#ifndef PARSER_IUNARYOPTAG_HPP_
#define PARSER_IUNARYOPTAG_HPP_

#include <string>

namespace ovum::compiler::parser {

class IUnaryOpTag {
public:
  virtual ~IUnaryOpTag() = default;

  virtual std::string_view Name() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IUNARYOPTAG_HPP_
