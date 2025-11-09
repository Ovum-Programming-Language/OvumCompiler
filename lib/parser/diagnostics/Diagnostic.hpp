#ifndef PARSER_DIAGNOSTIC_HPP_
#define PARSER_DIAGNOSTIC_HPP_

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "FixIt.hpp"
#include "RelatedInfo.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"
#include "severity/ISeverity.hpp"

namespace ovum::compiler::parser {

class Diagnostic {
public:
  Diagnostic();
  Diagnostic(std::shared_ptr<const ISeverity> sev, std::string code, std::string message);
  Diagnostic(const Diagnostic& other);
  Diagnostic(Diagnostic&& other) noexcept;
  Diagnostic& operator=(const Diagnostic& other);
  Diagnostic& operator=(Diagnostic&& other) noexcept;
  ~Diagnostic();

  void SetSeverity(std::shared_ptr<const ISeverity> sev);
  const std::shared_ptr<const ISeverity>& GetSeverity() const noexcept;

  void SetCode(std::string c);
  const std::string& GetCode() const noexcept;

  void SetMessage(std::string m);
  const std::string& GetMessage() const noexcept;

  void SetCategory(std::string cat);
  const std::string& GetCategory() const noexcept;

  void SetWhere(SourceSpan sp);
  void ResetWhere();
  const std::optional<SourceSpan>& GetWhere() const noexcept;

  void AddNote(RelatedInfo note);
  void ClearNotes();
  const std::vector<RelatedInfo>& GetNotes() const noexcept;

  void AddFix(FixIt fix);
  void ClearFixes();
  const std::vector<FixIt>& GetFixes() const noexcept;

  void SetFatal(bool on);
  bool IsFatal() const noexcept;

  void SetSuppressed(bool on);
  bool IsSuppressed() const noexcept;

private:
  std::shared_ptr<const ISeverity> severity_;
  std::string code_;
  std::string message_;
  std::string category_;
  std::optional<SourceSpan> where_;
  std::vector<RelatedInfo> notes_;
  std::vector<FixIt> fixes_;
  bool is_fatal_ = false;
  bool is_suppressed_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_DIAGNOSTIC_HPP_
