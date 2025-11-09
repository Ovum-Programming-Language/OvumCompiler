#include "lib/parser/ast/nodes/exprs/Unary.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void Unary::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const IUnaryOpTag& Unary::Op() const noexcept {
  return *op_;
}

void Unary::SetOp(const IUnaryOpTag& new_op) noexcept {
  op_ = &new_op;
}

const Expr& Unary::Operand() const noexcept {
  return *operand_;
}

Expr& Unary::MutableOperand() noexcept {
  return *operand_;
}

void Unary::SetOperand(std::unique_ptr<Expr> new_operand) {
  operand_ = std::move(new_operand);
}

std::unique_ptr<Expr> Unary::ReplaceOperand(std::unique_ptr<Expr> new_operand) {
  auto old_operand = std::move(operand_);
  operand_ = std::move(new_operand);
  return old_operand;
}
