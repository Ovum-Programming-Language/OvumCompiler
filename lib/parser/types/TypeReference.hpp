#ifndef TYPEREFERENCE_HPP_
#define TYPEREFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "Nullable.hpp"
#include "ResolvedTypeHandle.hpp"

class TypeReference {
public:
  TypeReference();
  explicit TypeReference(std::string name);
  explicit TypeReference(std::vector<std::string> qname);

  TypeReference(const TypeReference& ref);
  TypeReference(TypeReference&& ref) noexcept;

  ~TypeReference();

  TypeReference& operator=(const TypeReference& ref);
  TypeReference& operator=(TypeReference&& ref) noexcept;

  TypeReference& operator=(std::string name);

  bool StructurallyEquals(const TypeReference& other) const noexcept;

  std::string StableKey() const;

  std::string ToStringHuman() const {
    return StableKey();
  }

  const std::vector<std::string>& QualifiedName() const noexcept {
    return qname_;
  }

  std::string_view SimpleName() const noexcept;

  void SetQualifiedName(std::vector<std::string> qname);

  void SetSimpleName(std::string name);

  void PushQualifier(std::string qualifier);
  bool PopFrontQualifier();
  bool PopBackQualifier();

  const std::vector<TypeReference>& TypeArguments() const noexcept {
    return type_args_;
  }
  std::vector<TypeReference>& MutableTypeArguments() noexcept {
    return type_args_;
  }

  void ClearTypeArguments();
  void AddTypeArgument(TypeReference arg);
  std::size_t Arity() const noexcept {
    return type_args_.size();
  }

  const Nullable& Nullability() const noexcept {
    return nullable_;
  }
  bool IsNullable() const noexcept {
    return nullable_.IsOn();
  }
  void SetNullable(bool on) noexcept {
    nullable_.Set(on);
  }
  void MakeNullable() noexcept {
    nullable_.Enable();
  }
  void MakeNonNullable() noexcept {
    nullable_.Disable();
  }

  TypeReference WithoutNullable() const;

  bool IsResolved() const noexcept {
    return static_cast<bool>(resolved_);
  }
  const ResolvedTypeHandle* Resolved() const noexcept {
    return resolved_ ? resolved_.get() : nullptr;
  }

  void SetResolvedHandle(const void* decl, std::string mangled, bool is_interface);

  void ResetResolvedHandle() noexcept {
    resolved_.reset();
  }

private:
  static std::string JoinQualified(const std::vector<std::string>& parts, std::string_view sep);
  static std::string ArgsToString(const std::vector<TypeReference>& args);

  void InvalidateResolution() noexcept {
    resolved_.reset();
  }
  std::vector<std::string> qname_;
  std::vector<TypeReference> type_args_;
  Nullable nullable_;
  std::unique_ptr<ResolvedTypeHandle> resolved_;
};

#endif // TYPEREFERENCE_HPP_
