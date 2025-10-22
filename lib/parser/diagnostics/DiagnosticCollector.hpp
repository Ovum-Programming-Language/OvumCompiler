#ifndef DIAGNOSTICCOLLECTOR_HPP_
#define DIAGNOSTICCOLLECTOR_HPP_

#include <cstddef>
#include <functional>
#include <unordered_set>

#include "IDiagnosticSink.hpp"

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
  void EnableDeduplication(bool on);
  void SetCapacity(std::size_t max_total);
  void SetErrorLimit(std::size_t max_errors);
  void SetWarningLimit(std::size_t max_warnings);

  bool IsSuppressed(const Diagnostic& d) const;

private:
  bool ShouldKeep(const Diagnostic& d) const;
  bool IsDuplicate(const Diagnostic& d) const;

  std::vector<Diagnostic> diags_;
  std::unordered_set<std::string> suppressed_codes_;
  std::unordered_set<std::string> suppressed_categories_;
  Predicate global_filter_{};

  bool dedup_ = true;
  std::size_t capacity_ = 0;      // 0 == without limits
  std::size_t error_limit_ = 0;   // 0 == without limits
  std::size_t warning_limit_ = 0; // 0 == without limits

  std::size_t errors_ = 0;
  std::size_t warnings_ = 0;
};

#endif // DIAGNOSTICCOLLECTOR_HPP_
