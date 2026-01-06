#include "NamespaceRef.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void NamespaceRef::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& NamespaceRef::NamespaceExpr() const noexcept {
  return *namespace_;
}

Expr& NamespaceRef::MutableNamespaceExpr() noexcept {
  return *namespace_;
}

void NamespaceRef::SetNamespaceExpr(std::unique_ptr<Expr> new_namespace_expr) {
  namespace_ = std::move(new_namespace_expr);
}

std::unique_ptr<Expr> NamespaceRef::ReplaceNamespaceExpr(std::unique_ptr<Expr> new_namespace_expr) {
  auto old_ns = std::move(namespace_);
  namespace_ = std::move(new_namespace_expr);
  return old_ns;
}

const std::string& NamespaceRef::Name() const noexcept {
  return name_;
}

void NamespaceRef::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

} // namespace ovum::compiler::parser
