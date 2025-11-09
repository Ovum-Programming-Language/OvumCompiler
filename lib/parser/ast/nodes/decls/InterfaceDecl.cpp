#include "InterfaceDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void InterfaceDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& InterfaceDecl::Name() const noexcept {
  return name_;
}

void InterfaceDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const std::vector<std::unique_ptr<InterfaceMethod>>& InterfaceDecl::Members() const noexcept {
  return methods_;
}

std::vector<std::unique_ptr<InterfaceMethod>>& InterfaceDecl::MutableMembers() noexcept {
  return methods_;
}

void InterfaceDecl::AddMember(std::unique_ptr<InterfaceMethod> method) {
  methods_.emplace_back(std::move(method));
}

std::unique_ptr<InterfaceMethod> InterfaceDecl::ReleaseMember(std::size_t index) {
  if (index >= methods_.size()) {
    return nullptr;
  }

  auto old = std::move(methods_[index]);
  methods_.erase(methods_.begin() + static_cast<std::ptrdiff_t>(index));
  return old;
}
