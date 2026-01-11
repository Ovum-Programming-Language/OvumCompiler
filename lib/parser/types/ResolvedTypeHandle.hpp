#ifndef PARSER_RESOLVEDTYPEHANDLE_HPP_
#define PARSER_RESOLVEDTYPEHANDLE_HPP_

#include <string>

namespace ovum::compiler::parser {

class ResolvedTypeHandle {
public:
  ResolvedTypeHandle() = default;
  ResolvedTypeHandle(const void* decl, std::string mangled, bool is_interface);

  ResolvedTypeHandle(const ResolvedTypeHandle& other) = default;
  ResolvedTypeHandle(ResolvedTypeHandle&& other) noexcept = default;
  ~ResolvedTypeHandle() = default;

  ResolvedTypeHandle& operator=(const ResolvedTypeHandle& other) = default;
  ResolvedTypeHandle& operator=(ResolvedTypeHandle&& other) noexcept = default;

  [[nodiscard]] const void* Decl() const noexcept;
  [[nodiscard]] std::string_view Mangled() const noexcept;
  [[nodiscard]] bool IsInterface() const noexcept;
  [[nodiscard]] bool IsValid() const noexcept;

private:
  const void* decl_ = nullptr;
  std::string mangled_;
  bool is_interface_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_RESOLVEDTYPEHANDLE_HPP_
