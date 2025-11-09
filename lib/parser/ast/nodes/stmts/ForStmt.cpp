#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void ForStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& ForStmt::IteratorName() const noexcept {
  return iter_name_;
}

void ForStmt::SetIteratorName(std::string new_name) {
  iter_name_ = std::move(new_name);
}

const Expr* ForStmt::IteratorExpr() const noexcept {
  return iter_expr_.get();
}

Expr* ForStmt::MutableIteratorExpr() noexcept {
  return iter_expr_.get();
}

void ForStmt::SetIteratorExpr(std::unique_ptr<Expr> expression) {
  iter_expr_ = std::move(expression);
}

std::unique_ptr<Expr> ForStmt::ReleaseIteratorExpr() {
  return std::move(iter_expr_);
}

const Block* ForStmt::Body() const noexcept {
  return body_.get();
}

Block* ForStmt::MutableBody() noexcept {
  return body_.get();
}

void ForStmt::SetBody(std::unique_ptr<Block> body_block) {
  body_ = std::move(body_block);
}

std::unique_ptr<Block> ForStmt::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
