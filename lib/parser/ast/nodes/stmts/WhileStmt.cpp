#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void WhileStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr* WhileStmt::Condition() const noexcept {
  return cond_.get();
}

Expr* WhileStmt::MutableCondition() noexcept {
  return cond_.get();
}

void WhileStmt::SetCondition(std::unique_ptr<Expr> condition_expr) {
  cond_ = std::move(condition_expr);
}

std::unique_ptr<Expr> WhileStmt::ReleaseCondition() {
  return std::move(cond_);
}

const Block* WhileStmt::Body() const noexcept {
  return body_.get();
}

Block* WhileStmt::MutableBody() noexcept {
  return body_.get();
}

void WhileStmt::SetBody(std::unique_ptr<Block> body_block) {
  body_ = std::move(body_block);
}

std::unique_ptr<Block> WhileStmt::ReleaseBody() {
  return std::move(body_);
}
