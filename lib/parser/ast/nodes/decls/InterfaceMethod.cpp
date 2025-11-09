#include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void InterfaceMethod::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& InterfaceMethod::Name() const noexcept {
  return name_;
}

void InterfaceMethod::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const std::vector<InterfaceMethod::Param>& InterfaceMethod::Params() const noexcept {
  return params_;
}

std::vector<InterfaceMethod::Param>& InterfaceMethod::MutableParams() noexcept {
  return params_;
}

const TypeReference* InterfaceMethod::ReturnType() const noexcept {
  return ret_type_.get();
}

void InterfaceMethod::SetReturnType(std::unique_ptr<TypeReference> type) {
  ret_type_ = std::move(type);
}
