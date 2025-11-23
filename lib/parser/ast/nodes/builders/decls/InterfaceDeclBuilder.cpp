#include "InterfaceDeclBuilder.hpp"

namespace ovum::compiler::parser {

InterfaceDeclBuilder& InterfaceDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

InterfaceDeclBuilder& InterfaceDeclBuilder::AddMethod(std::unique_ptr<InterfaceMethod> method) {
  node_->MutableMembers().push_back(std::move(method));
  return *this;
}

} // namespace ovum::compiler::parser
