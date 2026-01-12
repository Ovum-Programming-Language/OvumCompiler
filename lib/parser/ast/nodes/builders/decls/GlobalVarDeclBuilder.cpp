#include "GlobalVarDeclBuilder.hpp"

namespace ovum::compiler::parser {

GlobalVarDeclBuilder& GlobalVarDeclBuilder::WithVar(bool is_var) {
  node_->SetVar(is_var);
  return *this;
}

GlobalVarDeclBuilder& GlobalVarDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

GlobalVarDeclBuilder& GlobalVarDeclBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

GlobalVarDeclBuilder& GlobalVarDeclBuilder::WithInit(std::unique_ptr<Expr> init) {
  node_->SetInit(std::move(init));
  return *this;
}

GlobalVarDeclBuilder& GlobalVarDeclBuilder::WithIsVar(bool is_var) {
  return WithVar(is_var);
}

} // namespace ovum::compiler::parser
