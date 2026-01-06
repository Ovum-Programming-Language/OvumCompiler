#include "Elvis.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void Elvis::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& Elvis::Lhs() const noexcept {
  return *lhs_;
}

Expr& Elvis::MutableLhs() noexcept {
  return *lhs_;
}

void Elvis::SetLhs(std::unique_ptr<Expr> new_lhs) {
  lhs_ = std::move(new_lhs);
}

std::unique_ptr<Expr> Elvis::ReplaceLhs(std::unique_ptr<Expr> new_lhs) {
  auto old_lhs = std::move(lhs_);
  lhs_ = std::move(new_lhs);
  return old_lhs;
}

const Expr& Elvis::Rhs() const noexcept {
  return *rhs_;
}

Expr& Elvis::MutableRhs() noexcept {
  return *rhs_;
}

void Elvis::SetRhs(std::unique_ptr<Expr> new_rhs) {
  rhs_ = std::move(new_rhs);
}

std::unique_ptr<Expr> Elvis::ReplaceRhs(std::unique_ptr<Expr> new_rhs) {
  auto old_rhs = std::move(rhs_);
  rhs_ = std::move(new_rhs);
  return old_rhs;
}

} // namespace ovum::compiler::parser
