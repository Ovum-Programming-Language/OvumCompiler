#include "MethodDeclBuilder.hpp"

namespace ovum::compiler::parser {

MethodDeclBuilder& MethodDeclBuilder::WithPublic(bool is_public) {
  node_->SetPublic(is_public);
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithOverride(bool is_override) {
  node_->SetOverride(is_override);
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithStatic(bool is_static) {
  node_->SetStatic(is_static);
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithPure(bool is_pure) {
  node_->SetPure(is_pure);
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::AddParam(Param parameter) {
  node_->MutableParams().push_back(std::move(parameter));
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithReturnType(std::unique_ptr<TypeReference> type) {
  node_->SetReturnType(std::move(type));
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser
