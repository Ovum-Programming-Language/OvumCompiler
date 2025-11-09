#ifndef TYPEREFERENCE_HPP_
#define TYPEREFERENCE_HPP_

#include <memory>
#include <string>
#include <string_view>
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
  std::string ToStringHuman() const;

  const std::vector<std::string>& QualifiedName() const noexcept;
  std::string_view SimpleName() const noexcept;

  void SetQualifiedName(std::vector<std::string> qname);
  void SetSimpleName(std::string name);
  void PushQualifier(std::string qualifier);
  bool PopFrontQualifier();
  bool PopBackQualifier();

  const std::vector<TypeReference>& TypeArguments() const noexcept;
  std::vector<TypeReference>& MutableTypeArguments() noexcept;

  void ClearTypeArguments();
  void AddTypeArgument(TypeReference arg);
  std::size_t Arity() const noexcept;

  const Nullable& Nullability() const noexcept;
  bool IsNullable() const noexcept;
  void SetNullable(bool on) noexcept;
  void MakeNullable() noexcept;
  void MakeNonNullable() noexcept;

  TypeReference WithoutNullable() const;

  bool IsResolved() const noexcept;
  const ResolvedTypeHandle* Resolved() const noexcept;

  void SetResolvedHandle(const void* decl, std::string mangled, bool is_interface);
  void ResetResolvedHandle() noexcept;

private:
  static std::string JoinQualified(const std::vector<std::string>& parts);
  static std::string ArgsToString(const std::vector<TypeReference>& args);

  void InvalidateResolution() noexcept;

  std::vector<std::string> qname_;
  std::vector<TypeReference> type_args_;
  Nullable nullable_;
  std::unique_ptr<ResolvedTypeHandle> resolved_;
};

#endif // TYPEREFERENCE_HPP_
