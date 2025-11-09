#include "lib/parser/ast/nodes/exprs/IndexAccess.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void IndexAccess::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& IndexAccess::Object() const noexcept {
  return *object_;
}

Expr& IndexAccess::MutableObject() noexcept {
  return *object_;
}

void IndexAccess::SetObject(std::unique_ptr<Expr> new_object) {
  object_ = std::move(new_object);
}

std::unique_ptr<Expr> IndexAccess::ReplaceObject(std::unique_ptr<Expr> new_object) {
  auto old_object = std::move(object_);
  object_ = std::move(new_object);
  return old_object;
}

const Expr& IndexAccess::IndexExpr() const noexcept {
  return *index_;
}

Expr& IndexAccess::MutableIndexExpr() noexcept {
  return *index_;
}

void IndexAccess::SetIndexExpr(std::unique_ptr<Expr> new_index) {
  index_ = std::move(new_index);
}

std::unique_ptr<Expr> IndexAccess::ReplaceIndexExpr(std::unique_ptr<Expr> new_index) {
  auto old_index = std::move(index_);
  index_ = std::move(new_index);
  return old_index;
}
