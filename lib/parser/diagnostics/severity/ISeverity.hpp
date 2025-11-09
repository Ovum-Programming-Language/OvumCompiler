#ifndef PARSER_ISEVERITY_HPP_
#define PARSER_ISEVERITY_HPP_

#include <string_view>

namespace ovum::compiler::parser {

class ISeverity {
public:
  virtual ~ISeverity() = default;
  virtual std::string_view Name() const = 0;
  virtual int Level() const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ISEVERITY_HPP_
