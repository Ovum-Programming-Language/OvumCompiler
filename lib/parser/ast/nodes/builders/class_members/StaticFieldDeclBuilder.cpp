#include "StaticFieldDeclBuilder.hpp"

namespace ovum::compiler::parser {

StaticFieldDeclBuilder& StaticFieldDeclBuilder::WithPublic(bool is_public) {
  node_->SetPublic(is_public);
  return *this;
}

StaticFieldDeclBuilder& StaticFieldDeclBuilder::WithVar(bool is_var) {
  node_->SetVar(is_var);
  return *this;
}

StaticFieldDeclBuilder& StaticFieldDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

StaticFieldDeclBuilder& StaticFieldDeclBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

StaticFieldDeclBuilder& StaticFieldDeclBuilder::WithInit(std::unique_ptr<Expr> init) {
  node_->SetInit(std::move(init));
  return *this;
}

} // namespace ovum::compiler::parser
