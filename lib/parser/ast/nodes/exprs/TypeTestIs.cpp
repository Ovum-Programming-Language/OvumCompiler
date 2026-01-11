#include "TypeTestIs.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void TypeTestIs::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& TypeTestIs::Expression() const noexcept {
  return *expr_;
}

Expr& TypeTestIs::MutableExpression() const noexcept {
  return *expr_;
}

void TypeTestIs::SetExpression(std::unique_ptr<Expr> new_expression) {
  expr_ = std::move(new_expression);
}

std::unique_ptr<Expr> TypeTestIs::ReplaceExpression(std::unique_ptr<Expr> new_expression) {
  auto old_expr = std::move(expr_);
  expr_ = std::move(new_expression);
  return old_expr;
}

const TypeReference& TypeTestIs::Type() const noexcept {
  return type_;
}

TypeReference& TypeTestIs::MutableType() noexcept {
  return type_;
}

void TypeTestIs::SetType(TypeReference new_type) {
  type_ = std::move(new_type);
}

} // namespace ovum::compiler::parser
