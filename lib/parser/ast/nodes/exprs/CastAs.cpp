#include "lib/parser/ast/nodes/exprs/CastAs.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void CastAs::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& CastAs::Expression() const noexcept {
  return *expr_;
}

Expr& CastAs::MutableExpression() noexcept {
  return *expr_;
}

void CastAs::SetExpression(std::unique_ptr<Expr> new_expression) {
  expr_ = std::move(new_expression);
}

std::unique_ptr<Expr> CastAs::ReplaceExpression(std::unique_ptr<Expr> new_expression) {
  auto old_expr = std::move(expr_);
  expr_ = std::move(new_expression);
  return old_expr;
}

const TypeReference& CastAs::Type() const noexcept {
  return type_;
}

TypeReference& CastAs::MutableType() noexcept {
  return type_;
}

void CastAs::SetType(TypeReference new_type) {
  type_ = std::move(new_type);
}

} // namespace ovum::compiler::parser
