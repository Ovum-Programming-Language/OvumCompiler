#include "InterfaceDeclBuilder.hpp"

namespace ovum::compiler::parser {

InterfaceDeclBuilder& InterfaceDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

InterfaceDeclBuilder& InterfaceDeclBuilder::AddMethod(std::unique_ptr<InterfaceMethod> method) {
  node_->AddMember(std::move(method));
  return *this;
}

InterfaceDeclBuilder& InterfaceDeclBuilder::WithMethods(std::vector<std::unique_ptr<InterfaceMethod>> methods) {
  for (auto& method : methods) {
    AddMethod(std::move(method));
  }
  return *this;
}

} // namespace ovum::compiler::parser
