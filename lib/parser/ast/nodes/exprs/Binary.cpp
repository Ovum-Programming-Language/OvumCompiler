#include "Binary.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void Binary::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const IBinaryOpTag& Binary::Op() const noexcept {
  return *op_;
}

void Binary::SetOp(const IBinaryOpTag& new_op) noexcept {
  op_ = &new_op;
}

const Expr& Binary::Lhs() const noexcept {
  return *lhs_;
}

Expr& Binary::MutableLhs() noexcept {
  return *lhs_;
}

void Binary::SetLhs(std::unique_ptr<Expr> new_lhs) {
  lhs_ = std::move(new_lhs);
}

std::unique_ptr<Expr> Binary::ReplaceLhs(std::unique_ptr<Expr> new_lhs) {
  auto old_lhs = std::move(lhs_);
  lhs_ = std::move(new_lhs);
  return old_lhs;
}

const Expr& Binary::Rhs() const noexcept {
  return *rhs_;
}

Expr& Binary::MutableRhs() noexcept {
  return *rhs_;
}

void Binary::SetRhs(std::unique_ptr<Expr> new_rhs) {
  rhs_ = std::move(new_rhs);
}

std::unique_ptr<Expr> Binary::ReplaceRhs(std::unique_ptr<Expr> new_rhs) {
  auto old_rhs = std::move(rhs_);
  rhs_ = std::move(new_rhs);
  return old_rhs;
}

} // namespace ovum::compiler::parser
