#include "ExprStmtBuilder.hpp"

namespace ovum::compiler::parser {

ExprStmtBuilder& ExprStmtBuilder::WithExpr(std::unique_ptr<Expr> expr) {
  node_->SetExpression(std::move(expr));
  return *this;
}

} // namespace ovum::compiler::parser