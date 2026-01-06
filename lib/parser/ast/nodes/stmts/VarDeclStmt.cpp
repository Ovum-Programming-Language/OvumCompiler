#include "VarDeclStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void VarDeclStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool VarDeclStmt::IsVar() const noexcept {
  return is_var_;
}

void VarDeclStmt::SetVar(bool is_var) noexcept {
  is_var_ = is_var;
}

const std::string& VarDeclStmt::Name() const noexcept {
  return name_;
}

void VarDeclStmt::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const TypeReference& VarDeclStmt::Type() const noexcept {
  return type_;
}

TypeReference& VarDeclStmt::MutableType() noexcept {
  return type_;
}

void VarDeclStmt::SetType(TypeReference new_type) {
  type_ = std::move(new_type);
}

const Expr* VarDeclStmt::Init() const noexcept {
  return init_.get();
}

Expr* VarDeclStmt::MutableInit() noexcept {
  return init_.get();
}

void VarDeclStmt::SetInit(std::unique_ptr<Expr> init_expr) {
  init_ = std::move(init_expr);
}

std::unique_ptr<Expr> VarDeclStmt::ReleaseInit() {
  return std::move(init_);
}

} // namespace ovum::compiler::parser
