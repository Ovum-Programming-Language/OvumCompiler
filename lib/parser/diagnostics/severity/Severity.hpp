#ifndef SEVERITY_HPP_
#define SEVERITY_HPP_
#include "ISeverity.hpp"

class Severity {
public:
  static const ISeverity& Note();    // Level=0
  static const ISeverity& Warning(); // Level=1
  static const ISeverity& Error();   // Level=2

  static const ISeverity& Custom(std::string_view name, int level);
};

#endif // SEVERITY_HPP_
