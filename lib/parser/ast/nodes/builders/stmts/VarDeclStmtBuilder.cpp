#include "VarDeclStmtBuilder.hpp"

namespace ovum::compiler::parser {

VarDeclStmtBuilder& VarDeclStmtBuilder::WithVar(bool is_var) {
  node_->SetVar(is_var);
  return *this;
}

VarDeclStmtBuilder& VarDeclStmtBuilder::WithIsVar(bool is_var) {
  return WithVar(is_var);
}

VarDeclStmtBuilder& VarDeclStmtBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

VarDeclStmtBuilder& VarDeclStmtBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

VarDeclStmtBuilder& VarDeclStmtBuilder::WithInit(std::unique_ptr<Expr> init) {
  node_->SetInit(std::move(init));
  return *this;
}

} // namespace ovum::compiler::parser
