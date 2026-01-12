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

ClassDeclBuilder& ClassDeclBuilder::WithImplements(std::vector<TypeReference> implements) {
  for (auto& impl : implements) {
    node_->AddImplements(std::move(impl));
  }
  return *this;
}

ClassDeclBuilder& ClassDeclBuilder::WithMembers(std::vector<std::unique_ptr<Decl>> members) {
  for (auto& member : members) {
    node_->AddMember(std::move(member));
  }
  return *this;
}

} // namespace ovum::compiler::parser
