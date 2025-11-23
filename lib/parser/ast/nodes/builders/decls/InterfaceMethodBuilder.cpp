#include "InterfaceMethodBuilder.hpp"

namespace ovum::compiler::parser {

InterfaceMethodBuilder& InterfaceMethodBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

InterfaceMethodBuilder& InterfaceMethodBuilder::AddParam(InterfaceMethod::Param parameter) {
  node_->MutableParams().push_back(std::move(parameter));
  return *this;
}

InterfaceMethodBuilder& InterfaceMethodBuilder::WithReturnType(std::unique_ptr<TypeReference> type) {
  node_->SetReturnType(std::move(type));
  return *this;
}

} // namespace ovum::compiler::parser
