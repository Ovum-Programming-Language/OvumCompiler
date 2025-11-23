#include "ClassDeclBuilder.hpp"

namespace ovum::compiler::parser {

ClassDeclBuilder& ClassDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

ClassDeclBuilder& ClassDeclBuilder::AddImplements(TypeReference iface) {
  node_->MutableImplements().push_back(std::move(iface));
  return *this;
}

ClassDeclBuilder& ClassDeclBuilder::AddMember(std::unique_ptr<Decl> member) {
  node_->MutableMembers().push_back(std::move(member));
  return *this;
}

} // namespace ovum::compiler::parser
