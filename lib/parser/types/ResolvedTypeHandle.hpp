#ifndef RESOLVEDTYPEHANDLE_HPP_
#define RESOLVEDTYPEHANDLE_HPP_

#include <string>

class ResolvedTypeHandle {
public:
  ResolvedTypeHandle() = default;
  ResolvedTypeHandle(const void* decl, std::string mangled, bool is_interface);

  ResolvedTypeHandle(const ResolvedTypeHandle& other) = default;
  ResolvedTypeHandle(ResolvedTypeHandle&& other) noexcept = default;
  ~ResolvedTypeHandle() = default;

  ResolvedTypeHandle& operator=(const ResolvedTypeHandle& other) = default;
  ResolvedTypeHandle& operator=(ResolvedTypeHandle&& other) noexcept = default;

  const void* Decl() const noexcept;
  std::string_view Mangled() const noexcept;
  bool IsInterface() const noexcept;
  bool IsValid() const noexcept;

private:
  const void* decl_ = nullptr;
  std::string mangled_;
  bool is_interface_ = false;
};

#endif // RESOLVEDTYPEHANDLE_HPP_
