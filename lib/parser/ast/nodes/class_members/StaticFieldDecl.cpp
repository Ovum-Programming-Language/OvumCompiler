#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void StaticFieldDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool StaticFieldDecl::IsPublic() const noexcept {
  return is_public_;
}

void StaticFieldDecl::SetPublic(bool is_public) noexcept {
  is_public_ = is_public;
}

bool StaticFieldDecl::IsVar() const noexcept {
  return is_var_;
}

void StaticFieldDecl::SetVar(bool is_var) noexcept {
  is_var_ = is_var;
}

const std::string& StaticFieldDecl::Name() const noexcept {
  return name_;
}

void StaticFieldDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const TypeReference& StaticFieldDecl::Type() const noexcept {
  return type_;
}

TypeReference& StaticFieldDecl::MutableType() noexcept {
  return type_;
}

void StaticFieldDecl::SetType(TypeReference type) {
  type_ = std::move(type);
}

const Expr* StaticFieldDecl::Init() const noexcept {
  return init_.get();
}

Expr* StaticFieldDecl::MutableInit() noexcept {
  return init_.get();
}

void StaticFieldDecl::SetInit(std::unique_ptr<Expr> expr) {
  init_ = std::move(expr);
}

std::unique_ptr<Expr> StaticFieldDecl::ReleaseInit() {
  return std::move(init_);
}

} // namespace ovum::compiler::parser
