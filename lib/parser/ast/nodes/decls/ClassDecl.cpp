#include "ClassDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void ClassDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& ClassDecl::Name() const noexcept {
  return name_;
}

void ClassDecl::SetName(std::string name) {
  name_ = std::move(name);
}

const std::vector<TypeReference>& ClassDecl::Implements() const noexcept {
  return implements_;
}

std::vector<TypeReference>& ClassDecl::MutableImplements() noexcept {
  return implements_;
}

void ClassDecl::AddImplements(TypeReference type) {
  implements_.emplace_back(std::move(type));
}

const std::vector<std::unique_ptr<Decl>>& ClassDecl::Members() const noexcept {
  return members_;
}

std::vector<std::unique_ptr<Decl>>& ClassDecl::MutableMembers() noexcept {
  return members_;
}

void ClassDecl::AddMember(std::unique_ptr<Decl> decl) {
  members_.emplace_back(std::move(decl));
}

std::unique_ptr<Decl> ClassDecl::ReleaseMember(std::size_t index) {
  if (index >= members_.size()) {
    return nullptr;
  }

  auto old = std::move(members_[index]);
  members_.erase(members_.begin() + static_cast<std::ptrdiff_t>(index));
  return old;
}

} // namespace ovum::compiler::parser
