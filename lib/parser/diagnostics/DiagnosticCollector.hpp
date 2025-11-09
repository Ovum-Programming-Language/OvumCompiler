#ifndef PARSER_DIAGNOSTICCOLLECTOR_HPP_
#define PARSER_DIAGNOSTICCOLLECTOR_HPP_

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "IDiagnosticSink.hpp"

namespace ovum::compiler::parser {

class DiagnosticCollector : public IDiagnosticSink {
public:
  using Predicate = std::function<bool(const Diagnostic&)>;

  ~DiagnosticCollector() override = default;

  void Report(Diagnostic d) override;
  bool HasErrors() const override;
  std::size_t Count() const override;
  std::size_t ErrorCount() const override;
  std::size_t WarningCount() const override;

  void Note(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) override;
  void Warn(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) override;
  void Error(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) override;

  const std::vector<Diagnostic>& All() const;
  void Clear();

  void SuppressCode(std::string code);
  void SuppressCategory(std::string category);
  void SetGlobalFilter(Predicate p);
  void ResetGlobalFilter();
  void EnableDeduplication(bool on);

  void SetCapacity(std::optional<std::size_t> max_total);
  void SetErrorLimit(std::optional<std::size_t> max_errors);
  void SetWarningLimit(std::optional<std::size_t> max_warnings);

  bool IsSuppressed(const Diagnostic& d) const;

private:
  bool ShouldKeep(const Diagnostic& d) const;
  bool IsDuplicate(const Diagnostic& d) const;

  std::vector<Diagnostic> diags_;
  std::unordered_set<std::string> suppressed_codes_;
  std::unordered_set<std::string> suppressed_categories_;
  std::optional<Predicate> global_filter_;

  bool dedup_ = true;
  std::optional<std::size_t> capacity_;
  std::optional<std::size_t> error_limit_;
  std::optional<std::size_t> warning_limit_;

  std::size_t errors_ = 0;
  std::size_t warnings_ = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_DIAGNOSTICCOLLECTOR_HPP_
