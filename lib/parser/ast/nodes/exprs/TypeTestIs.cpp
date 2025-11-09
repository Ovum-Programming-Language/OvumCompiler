#include "lib/parser/ast/nodes/exprs/TypeTestIs.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void TypeTestIs::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& TypeTestIs::Expression() const noexcept {
  return *expr;
}

Expr& TypeTestIs::MutableExpression() noexcept {
  return *expr;
}

void TypeTestIs::SetExpression(std::unique_ptr<Expr> new_expression) {
  expr = std::move(new_expression);
}

std::unique_ptr<Expr> TypeTestIs::ReplaceExpression(std::unique_ptr<Expr> new_expression) {
  auto old_expr = std::move(expr);
  expr = std::move(new_expression);
  return old_expr;
}

const TypeReference& TypeTestIs::Type() const noexcept {
  return type;
}

TypeReference& TypeTestIs::MutableType() noexcept {
  return type;
}

void TypeTestIs::SetType(TypeReference new_type) {
  type = std::move(new_type);
}
