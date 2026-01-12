#include "Diagnostic.hpp"

#include <utility>

namespace ovum::compiler::parser {

Diagnostic::Diagnostic() = default;

Diagnostic::Diagnostic(std::shared_ptr<const ISeverity> sev, std::string code, std::string message) :
    severity_(std::move(sev)), code_(std::move(code)), message_(std::move(message)) {
}

Diagnostic::Diagnostic(const Diagnostic& other) = default;

Diagnostic::Diagnostic(Diagnostic&& other) noexcept :
    severity_(std::move(other.severity_)), code_(std::move(other.code_)), message_(std::move(other.message_)),
    category_(std::move(other.category_)), where_(std::move(other.where_)), notes_(std::move(other.notes_)),
    fixes_(std::move(other.fixes_)), is_fatal_(other.is_fatal_), is_suppressed_(other.is_suppressed_) {
}

Diagnostic& Diagnostic::operator=(const Diagnostic& other) {
  if (this == &other) {
    return *this;
  }

  severity_ = other.severity_;
  code_ = other.code_;
  message_ = other.message_;
  category_ = other.category_;
  where_ = other.where_;
  notes_ = other.notes_;
  fixes_ = other.fixes_;
  is_fatal_ = other.is_fatal_;
  is_suppressed_ = other.is_suppressed_;
  return *this;
}

Diagnostic& Diagnostic::operator=(Diagnostic&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  severity_ = std::move(other.severity_);
  code_ = std::move(other.code_);
  message_ = std::move(other.message_);
  category_ = std::move(other.category_);
  where_ = std::move(other.where_);
  notes_ = std::move(other.notes_);
  fixes_ = std::move(other.fixes_);
  is_fatal_ = other.is_fatal_;
  is_suppressed_ = other.is_suppressed_;
  return *this;
}

Diagnostic::~Diagnostic() = default;

void Diagnostic::SetSeverity(std::shared_ptr<const ISeverity> sev) {
  severity_ = std::move(sev);
}
const std::shared_ptr<const ISeverity>& Diagnostic::GetSeverity() const noexcept {
  return severity_;
}

void Diagnostic::SetCode(std::string c) {
  code_ = std::move(c);
}
const std::string& Diagnostic::GetCode() const noexcept {
  return code_;
}

void Diagnostic::SetMessage(std::string m) {
  message_ = std::move(m);
}
const std::string& Diagnostic::GetDiagnosticsMessage() const noexcept {
  return message_;
}

void Diagnostic::SetCategory(std::string cat) {
  category_ = std::move(cat);
}
const std::string& Diagnostic::GetCategory() const noexcept {
  return category_;
}

void Diagnostic::SetWhere(SourceSpan sp) {
  where_ = std::move(sp);
}
void Diagnostic::ResetWhere() {
  where_.reset();
}
const std::optional<SourceSpan>& Diagnostic::GetWhere() const noexcept {
  return where_;
}

void Diagnostic::AddNote(RelatedInfo note) {
  notes_.emplace_back(std::move(note));
}
void Diagnostic::ClearNotes() {
  notes_.clear();
}
const std::vector<RelatedInfo>& Diagnostic::GetNotes() const noexcept {
  return notes_;
}

void Diagnostic::AddFix(FixIt fix) {
  fixes_.emplace_back(std::move(fix));
}
void Diagnostic::ClearFixes() {
  fixes_.clear();
}
const std::vector<FixIt>& Diagnostic::GetFixes() const noexcept {
  return fixes_;
}

void Diagnostic::SetFatal(bool on) {
  is_fatal_ = on;
}
bool Diagnostic::IsFatal() const noexcept {
  return is_fatal_;
}

void Diagnostic::SetSuppressed(bool on) {
  is_suppressed_ = on;
}
bool Diagnostic::IsSuppressed() const noexcept {
  return is_suppressed_;
}

} // namespace ovum::compiler::parser
