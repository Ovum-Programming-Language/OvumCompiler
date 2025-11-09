#include "lib/parser/types/ResolvedTypeHandle.hpp"

#include <string_view>

namespace ovum::compiler::parser {

ResolvedTypeHandle::ResolvedTypeHandle(const void* decl, std::string mangled, bool is_interface) :
    decl_(decl), mangled_(std::move(mangled)), is_interface_(is_interface) {
}

const void* ResolvedTypeHandle::Decl() const noexcept {
  return decl_;
}

std::string_view ResolvedTypeHandle::Mangled() const noexcept {
  return mangled_;
}

bool ResolvedTypeHandle::IsInterface() const noexcept {
  return is_interface_;
}

bool ResolvedTypeHandle::IsValid() const noexcept {
  return decl_ != nullptr;
}

} // namespace ovum::compiler::parser
