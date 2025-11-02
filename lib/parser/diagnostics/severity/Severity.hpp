#ifndef SEVERITY_HPP_
#define SEVERITY_HPP_

#include <memory>
#include <string_view>
#include "ISeverity.hpp"

class Severity {
public:
  static const std::shared_ptr<const ISeverity>& Note();
  static const std::shared_ptr<const ISeverity>& Warning();
  static const std::shared_ptr<const ISeverity>& Error();

  static std::shared_ptr<const ISeverity> Custom(std::string_view name, int level);
};

#endif // SEVERITY_HPP_
