#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void ReturnStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool ReturnStmt::HasValue() const noexcept {
  return value_.has_value() && static_cast<bool>(value_.value());
}

const Expr* ReturnStmt::Value() const noexcept {
  return value_.has_value() ? value_.value().get() : nullptr;
}

Expr* ReturnStmt::MutableValue() noexcept {
  return value_.has_value() ? value_.value().get() : nullptr;
}

void ReturnStmt::SetValue(std::unique_ptr<Expr> new_value) {
  value_.emplace(std::move(new_value));
}

void ReturnStmt::ResetValue() {
  value_.reset();
}

std::unique_ptr<Expr> ReturnStmt::ReleaseValue() {
  if (!value_.has_value()) {
    return nullptr;
  }

  auto out = std::move(value_.value());
  value_.reset();
  return out;
}

} // namespace ovum::compiler::parser
