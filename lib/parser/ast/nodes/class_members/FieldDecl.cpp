#include "FieldDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void FieldDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool FieldDecl::IsPublic() const noexcept {
  return is_public_;
}

void FieldDecl::SetPublic(bool value) noexcept {
  is_public_ = value;
}

bool FieldDecl::IsVar() const noexcept {
  return is_var_;
}

void FieldDecl::SetVar(bool var) noexcept {
  is_var_ = var;
}

const std::string& FieldDecl::Name() const noexcept {
  return name_;
}

void FieldDecl::SetName(std::string name) {
  name_ = std::move(name);
}

const TypeReference& FieldDecl::Type() const noexcept {
  return type_;
}

TypeReference& FieldDecl::MutableType() noexcept {
  return type_;
}

void FieldDecl::SetType(TypeReference type) {
  type_ = std::move(type);
}

const Expr* FieldDecl::Init() const noexcept {
  return init_.get();
}

Expr* FieldDecl::MutableInit() noexcept {
  return init_.get();
}

void FieldDecl::SetInit(std::unique_ptr<Expr> expr) {
  init_ = std::move(expr);
}

std::unique_ptr<Expr> FieldDecl::ReleaseInit() {
  return std::move(init_);
}

} // namespace ovum::compiler::parser
