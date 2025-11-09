#include "lib/parser/types/TypeReference.hpp"

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <utility>

TypeReference::TypeReference() = default;

TypeReference::TypeReference(std::string name) {
  qname_.clear();
  qname_.push_back(std::move(name));
}

TypeReference::TypeReference(std::vector<std::string> qname) : qname_(std::move(qname)) {
}

TypeReference::TypeReference(const TypeReference& ref) :
    qname_(ref.qname_), type_args_(ref.type_args_), nullable_(ref.nullable_) {
  if (ref.resolved_) {
    resolved_ = std::make_unique<ResolvedTypeHandle>(*ref.resolved_);
  }
}

TypeReference::TypeReference(TypeReference&& ref) noexcept :
    qname_(std::move(ref.qname_)), type_args_(std::move(ref.type_args_)), nullable_(ref.nullable_),
    resolved_(std::move(ref.resolved_)) {
}

TypeReference::~TypeReference() = default;

TypeReference& TypeReference::operator=(const TypeReference& ref) {
  if (this == &ref) {
    return *this;
  }

  qname_ = ref.qname_;
  type_args_ = ref.type_args_;
  nullable_ = ref.nullable_;

  if (ref.resolved_) {
    resolved_ = std::make_unique<ResolvedTypeHandle>(*ref.resolved_);
  } else {
    resolved_.reset();
  }

  return *this;
}

TypeReference& TypeReference::operator=(TypeReference&& ref) noexcept {
  if (this == &ref) {
    return *this;
  }

  qname_ = std::move(ref.qname_);
  type_args_ = std::move(ref.type_args_);
  nullable_ = ref.nullable_;
  resolved_ = std::move(ref.resolved_);
  return *this;
}

TypeReference& TypeReference::operator=(std::string name) {
  qname_.clear();
  qname_.push_back(std::move(name));
  InvalidateResolution();
  return *this;
}

bool TypeReference::StructurallyEquals(const TypeReference& other) const noexcept {
  if (nullable_.IsOn() != other.nullable_.IsOn()) {
    return false;
  }

  if (qname_ != other.qname_) {
    return false;
  }

  if (type_args_.size() != other.type_args_.size()) {
    return false;
  }

  for (std::size_t i = 0; i < type_args_.size(); ++i) {
    if (!type_args_[i].StructurallyEquals(other.type_args_[i])) {
      return false;
    }
  }
  return true;
}

std::string TypeReference::StableKey() const {
  std::ostringstream out;
  out << JoinQualified(qname_);
  if (!type_args_.empty()) {
    out << '<' << ArgsToString(type_args_) << '>';
  }

  if (nullable_.IsOn()) {
    out << '?';
  }

  return out.str();
}

std::string TypeReference::ToStringHuman() const {
  return StableKey();
}

const std::vector<std::string>& TypeReference::QualifiedName() const noexcept {
  return qname_;
}

std::string_view TypeReference::SimpleName() const noexcept {
  if (qname_.empty()) {
    return {};
  }

  return qname_.back();
}

void TypeReference::SetQualifiedName(std::vector<std::string> qname) {
  qname_ = std::move(qname);
  InvalidateResolution();
}

void TypeReference::SetSimpleName(std::string name) {
  if (qname_.empty()) {
    qname_.push_back(std::move(name));
  } else {
    qname_.back() = std::move(name);
  }

  InvalidateResolution();
}

void TypeReference::PushQualifier(std::string qualifier) {
  qname_.push_back(std::move(qualifier));
  InvalidateResolution();
}

bool TypeReference::PopFrontQualifier() {
  if (qname_.empty()) {
    return false;
  }

  qname_.erase(qname_.begin());
  InvalidateResolution();
  return true;
}

bool TypeReference::PopBackQualifier() {
  if (qname_.empty()) {
    return false;
  }

  qname_.pop_back();
  InvalidateResolution();
  return true;
}

const std::vector<TypeReference>& TypeReference::TypeArguments() const noexcept {
  return type_args_;
}

std::vector<TypeReference>& TypeReference::MutableTypeArguments() noexcept {
  return type_args_;
}

void TypeReference::ClearTypeArguments() {
  type_args_.clear();
  InvalidateResolution();
}

void TypeReference::AddTypeArgument(TypeReference arg) {
  type_args_.emplace_back(std::move(arg));
  InvalidateResolution();
}

std::size_t TypeReference::Arity() const noexcept {
  return type_args_.size();
}

const Nullable& TypeReference::Nullability() const noexcept {
  return nullable_;
}

bool TypeReference::IsNullable() const noexcept {
  return nullable_.IsOn();
}

void TypeReference::SetNullable(bool on) noexcept {
  nullable_.Set(on);
}

void TypeReference::MakeNullable() noexcept {
  nullable_.Enable();
}

void TypeReference::MakeNonNullable() noexcept {
  nullable_.Disable();
}

TypeReference TypeReference::WithoutNullable() const {
  TypeReference copy{*this};
  copy.MakeNonNullable();
  copy.InvalidateResolution();
  return copy;
}

bool TypeReference::IsResolved() const noexcept {
  return static_cast<bool>(resolved_);
}

const ResolvedTypeHandle* TypeReference::Resolved() const noexcept {
  return resolved_ ? resolved_.get() : nullptr;
}

void TypeReference::SetResolvedHandle(const void* decl, std::string mangled, bool is_interface) {
  resolved_ = std::make_unique<ResolvedTypeHandle>(decl, std::move(mangled), is_interface);
}

void TypeReference::ResetResolvedHandle() noexcept {
  resolved_.reset();
}

std::string TypeReference::JoinQualified(const std::vector<std::string>& parts) {
  if (parts.empty()) {
    return {};
  }

  std::ostringstream out;
  for (std::size_t i = 0; i < parts.size(); ++i) {
    if (i) {
      out << ".";
    }
    out << parts[i];
  }
  return out.str();
}

std::string TypeReference::ArgsToString(const std::vector<TypeReference>& args) {
  std::ostringstream out;
  for (std::size_t i = 0; i < args.size(); ++i) {
    if (i) {
      out << ", ";
    }

    out << args[i].StableKey();
  }
  return out.str();
}

void TypeReference::InvalidateResolution() noexcept {
  resolved_.reset();
}
