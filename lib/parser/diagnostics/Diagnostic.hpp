#ifndef DIAGNOSTIC_HPP_
#define DIAGNOSTIC_HPP_

#include <optional>
#include <string>
#include <vector>

#include "FixIt.hpp"
#include "RelatedInfo.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"
#include "severity/ISeverity.hpp"
#include "severity/Severity.hpp"

class Diagnostic {
  const ISeverity* severity = &Severity::Error();
  std::string code;
  std::string message;
  std::string category;

  std::optional<SourceSpan> where;
  std::vector<RelatedInfo> notes;
  std::vector<FixIt> fixes;

  bool is_fatal = false;
  bool is_suppressed = false;
};

#endif // DIAGNOSTIC_HPP_
