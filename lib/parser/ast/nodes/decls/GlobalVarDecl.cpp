#include "GlobalVarDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void GlobalVarDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool GlobalVarDecl::IsVar() const noexcept {
  return is_var_;
}

void GlobalVarDecl::SetVar(bool is_var) noexcept {
  is_var_ = is_var;
}

const std::string& GlobalVarDecl::Name() const noexcept {
  return name_;
}

void GlobalVarDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const TypeReference& GlobalVarDecl::Type() const noexcept {
  return type_;
}

TypeReference& GlobalVarDecl::MutableType() noexcept {
  return type_;
}

void GlobalVarDecl::SetType(TypeReference type) {
  type_ = std::move(type);
}

const Expr* GlobalVarDecl::Init() const noexcept {
  return init_.get();
}

Expr* GlobalVarDecl::MutableInit() noexcept {
  return init_.get();
}

void GlobalVarDecl::SetInit(std::unique_ptr<Expr> expr) {
  init_ = std::move(expr);
}

std::unique_ptr<Expr> GlobalVarDecl::ReleaseInit() {
  return std::move(init_);
}
