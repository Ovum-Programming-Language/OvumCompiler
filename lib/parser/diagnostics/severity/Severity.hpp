#ifndef PARSER_SEVERITY_HPP_
#define PARSER_SEVERITY_HPP_

#include <memory>
#include <string_view>

#include "ISeverity.hpp"

namespace ovum::compiler::parser {

class Severity {
public:
  static std::shared_ptr<const ISeverity> Note();
  static std::shared_ptr<const ISeverity> Warning();
  static std::shared_ptr<const ISeverity> Error();

  static std::shared_ptr<const ISeverity> Custom(std::string_view name, int level);
};


} // namespace ovum::compiler::parser

#endif // PARSER_SEVERITY_HPP_
