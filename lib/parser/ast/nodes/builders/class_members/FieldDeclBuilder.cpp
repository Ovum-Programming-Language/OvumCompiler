#include "FieldDeclBuilder.hpp"

namespace ovum::compiler::parser {

FieldDeclBuilder& FieldDeclBuilder::WithPublic(bool is_public) {
  node_->SetPublic(is_public);
  return *this;
}

FieldDeclBuilder& FieldDeclBuilder::WithVar(bool is_var) {
  node_->SetVar(is_var);
  return *this;
}

FieldDeclBuilder& FieldDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

FieldDeclBuilder& FieldDeclBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

FieldDeclBuilder& FieldDeclBuilder::WithInit(std::unique_ptr<Expr> init) {
  node_->SetInit(std::move(init));
  return *this;
}

FieldDeclBuilder& FieldDeclBuilder::WithIsPublic(bool is_public) {
  return WithPublic(is_public);
}

FieldDeclBuilder& FieldDeclBuilder::WithIsVar(bool is_var) {
  return WithVar(is_var);
}

} // namespace ovum::compiler::parser
