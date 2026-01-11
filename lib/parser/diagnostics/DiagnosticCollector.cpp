#include "lib/parser/diagnostics/DiagnosticCollector.hpp"

#include <utility>

#include "lib/parser/diagnostics/severity/Severity.hpp"

namespace ovum::compiler::parser {

void DiagnosticCollector::Report(Diagnostic d) {
  const bool suppressed = !ShouldKeep(d);
  d.SetSuppressed(suppressed);
  if (suppressed) {
    return;
  }

  if (dedup_ && IsDuplicate(d)) {
    return;
  }

  if (capacity_ && diags_.size() >= *capacity_) {
    return;
  }

  const int err_level = Severity::Error()->Level();
  const int warn_level = Severity::Warning()->Level();

  if (const int level = d.GetSeverity() ? d.GetSeverity()->Level() : 0; level >= err_level) {
    if (error_limit_ && errors_ >= *error_limit_) {
      return;
    }

    ++errors_;
  } else if (level >= warn_level) {
    if (warning_limit_ && warnings_ >= *warning_limit_) {
      return;
    }

    ++warnings_;
  }

  diags_.emplace_back(std::move(d));
}

bool DiagnosticCollector::HasErrors() const {
  return errors_ > 0;
}
std::size_t DiagnosticCollector::Count() const {
  return diags_.size();
}
std::size_t DiagnosticCollector::ErrorCount() const {
  return errors_;
}
std::size_t DiagnosticCollector::WarningCount() const {
  return warnings_;
}

void DiagnosticCollector::Note(std::string_view code, std::string_view msg, std::optional<SourceSpan> where) {
  Diagnostic d{Severity::Note(), std::string{code}, std::string{msg}};
  if (where) {
    d.SetWhere(*where);
  }

  Report(std::move(d));
}

void DiagnosticCollector::Warn(std::string_view code, std::string_view msg, std::optional<SourceSpan> where) {
  Diagnostic d{Severity::Warning(), std::string{code}, std::string{msg}};
  if (where) {
    d.SetWhere(*where);
  }

  Report(std::move(d));
}

void DiagnosticCollector::Error(std::string_view code, std::string_view msg, std::optional<SourceSpan> where) {
  Diagnostic d{Severity::Error(), std::string{code}, std::string{msg}};
  if (where) {
    d.SetWhere(*where);
  }

  Report(std::move(d));
}

const std::vector<Diagnostic>& DiagnosticCollector::All() const {
  return diags_;
}

void DiagnosticCollector::Clear() {
  diags_.clear();
  errors_ = 0;
  warnings_ = 0;
}

void DiagnosticCollector::SuppressCode(std::string code) {
  suppressed_codes_.insert(std::move(code));
}

void DiagnosticCollector::SuppressCategory(std::string category) {
  suppressed_categories_.insert(std::move(category));
}

void DiagnosticCollector::SetGlobalFilter(Predicate p) {
  global_filter_ = std::move(p);
}

void DiagnosticCollector::ResetGlobalFilter() {
  global_filter_.reset();
}

void DiagnosticCollector::EnableDeduplication(bool on) {
  dedup_ = on;
}

void DiagnosticCollector::SetCapacity(std::optional<std::size_t> max_total) {
  capacity_ = max_total;
}

void DiagnosticCollector::SetErrorLimit(std::optional<std::size_t> max_errors) {
  error_limit_ = max_errors;
}

void DiagnosticCollector::SetWarningLimit(std::optional<std::size_t> max_warnings) {
  warning_limit_ = max_warnings;
}

bool DiagnosticCollector::IsSuppressed(const Diagnostic& d) const {
  if (!d.GetCode().empty() && suppressed_codes_.contains(d.GetCode())) {
    return true;
  }

  if (!d.GetCategory().empty() && suppressed_categories_.contains(d.GetCategory())) {
    return true;
  }

  if (global_filter_ && !(*global_filter_)(d)) {
    return true;
  }

  return false;
}

bool DiagnosticCollector::ShouldKeep(const Diagnostic& d) const {
  return !IsSuppressed(d);
}

bool DiagnosticCollector::IsDuplicate(const Diagnostic& d) const {
  for (const auto& prev : diags_) {
    if (prev.GetCode() == d.GetCode() && prev.GetMessage() == d.GetMessage() &&
        (prev.GetSeverity() && d.GetSeverity() ? prev.GetSeverity()->Level() == d.GetSeverity()->Level()
                                               : prev.GetSeverity() == d.GetSeverity())) {
      return true;
    }
  }
  return false;
}

} // namespace ovum::compiler::parser
