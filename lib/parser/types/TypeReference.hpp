#ifndef PARSER_TYPEREFERENCE_HPP_
#define PARSER_TYPEREFERENCE_HPP_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "Nullable.hpp"
#include "ResolvedTypeHandle.hpp"

namespace ovum::compiler::parser {

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

  [[nodiscard]] bool StructurallyEquals(const TypeReference& other) const noexcept;

  [[nodiscard]] std::string StableKey() const;
  [[nodiscard]] std::string ToStringHuman() const;

  [[nodiscard]] const std::vector<std::string>& QualifiedName() const noexcept;
  [[nodiscard]] std::string_view SimpleName() const noexcept;

  void SetQualifiedName(std::vector<std::string> qname);
  void SetSimpleName(std::string name);
  void PushQualifier(std::string qualifier);
  bool PopFrontQualifier();
  bool PopBackQualifier();

  [[nodiscard]] const std::vector<TypeReference>& TypeArguments() const noexcept;
  std::vector<TypeReference>& MutableTypeArguments() noexcept;

  void ClearTypeArguments();
  void AddTypeArgument(TypeReference arg);
  [[nodiscard]] std::size_t Arity() const noexcept;

  [[nodiscard]] const Nullable& Nullability() const noexcept;
  [[nodiscard]] bool IsNullable() const noexcept;
  void SetNullable(bool on) noexcept;
  void MakeNullable() noexcept;
  void MakeNonNullable() noexcept;

  [[nodiscard]] TypeReference WithoutNullable() const;

  [[nodiscard]] bool IsResolved() const noexcept;
  [[nodiscard]] const ResolvedTypeHandle* Resolved() const noexcept;

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

} // namespace ovum::compiler::parser

#endif // PARSER_TYPEREFERENCE_HPP_
