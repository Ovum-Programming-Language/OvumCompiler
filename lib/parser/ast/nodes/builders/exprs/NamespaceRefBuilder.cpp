#include "NamespaceRefBuilder.hpp"

namespace ovum::compiler::parser {

NamespaceRefBuilder& NamespaceRefBuilder::WithNamespace(std::unique_ptr<Expr> ns) {
  node_->SetNamespaceExpr(std::move(ns));
  return *this;
}

NamespaceRefBuilder& NamespaceRefBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

} // namespace ovum::compiler::parser
