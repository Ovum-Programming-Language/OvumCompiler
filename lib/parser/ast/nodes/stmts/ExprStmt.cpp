#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void ExprStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr* ExprStmt::Expression() const noexcept {
  return expr_.get();
}

Expr* ExprStmt::MutableExpression() noexcept {
  return expr_.get();
}

void ExprStmt::SetExpression(std::unique_ptr<Expr> expression) {
  expr_ = std::move(expression);
}

std::unique_ptr<Expr> ExprStmt::ReleaseExpression() {
  return std::move(expr_);
}

} // namespace ovum::compiler::parser
