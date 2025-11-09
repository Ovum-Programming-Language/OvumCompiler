#include "lib/parser/ast/nodes/exprs/Assign.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void Assign::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const IAssignOpTag& Assign::Kind() const noexcept {
  return *kind_;
}

void Assign::SetKind(const IAssignOpTag& new_kind) noexcept {
  kind_ = &new_kind;
}

const Expr& Assign::Target() const noexcept {
  return *target_;
}

Expr& Assign::MutableTarget() noexcept {
  return *target_;
}

void Assign::SetTarget(std::unique_ptr<Expr> new_target) {
  target_ = std::move(new_target);
}

std::unique_ptr<Expr> Assign::ReplaceTarget(std::unique_ptr<Expr> new_target) {
  auto old_target = std::move(target_);
  target_ = std::move(new_target);
  return old_target;
}

const Expr& Assign::Value() const noexcept {
  return *value_;
}

Expr& Assign::MutableValue() noexcept {
  return *value_;
}

void Assign::SetValue(std::unique_ptr<Expr> new_value) {
  value_ = std::move(new_value);
}

std::unique_ptr<Expr> Assign::ReplaceValue(std::unique_ptr<Expr> new_value) {
  auto old_value = std::move(value_);
  value_ = std::move(new_value);
  return old_value;
}

} // namespace ovum::compiler::parser
