#include "FieldAccess.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void FieldAccess::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& FieldAccess::Object() const noexcept {
  return *object_;
}

Expr& FieldAccess::MutableObject() noexcept {
  return *object_;
}

void FieldAccess::SetObject(std::unique_ptr<Expr> new_object) {
  object_ = std::move(new_object);
}

std::unique_ptr<Expr> FieldAccess::ReplaceObject(std::unique_ptr<Expr> new_object) {
  auto old_object = std::move(object_);
  object_ = std::move(new_object);
  return old_object;
}

const std::string& FieldAccess::Name() const noexcept {
  return name_;
}

void FieldAccess::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

} // namespace ovum::compiler::parser
