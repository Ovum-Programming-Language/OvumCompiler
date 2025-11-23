#include "TypeTestIsBuilder.hpp"

namespace ovum::compiler::parser {

TypeTestIsBuilder& TypeTestIsBuilder::WithExpr(std::unique_ptr<Expr> expression) {
  node_->SetExpression(std::move(expression));
  return *this;
}

TypeTestIsBuilder& TypeTestIsBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

} // namespace ovum::compiler::parser
