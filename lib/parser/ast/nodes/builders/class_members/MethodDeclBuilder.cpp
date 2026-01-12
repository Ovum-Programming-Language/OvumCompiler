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

MethodDeclBuilder& MethodDeclBuilder::WithIsPure(bool is_pure) {
  return WithPure(is_pure);
}

MethodDeclBuilder& MethodDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::AddParam(Param parameter) {
  node_->MutableParams().push_back(std::move(parameter));
  return *this;
}

MethodDeclBuilder& MethodDeclBuilder::WithParams(std::vector<Param> params) {
  for (auto& param : params) {
    node_->MutableParams().push_back(std::move(param));
  }
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

MethodDeclBuilder& MethodDeclBuilder::WithIsPublic(bool is_public) {
  return WithPublic(is_public);
}

MethodDeclBuilder& MethodDeclBuilder::WithIsOverride(bool is_override) {
  return WithOverride(is_override);
}

MethodDeclBuilder& MethodDeclBuilder::WithIsStatic(bool is_static) {
  return WithStatic(is_static);
}

} // namespace ovum::compiler::parser
