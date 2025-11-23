#include "IndexAccessBuilder.hpp"

namespace ovum::compiler::parser {

IndexAccessBuilder& IndexAccessBuilder::WithObject(std::unique_ptr<Expr> object) {
  node_->SetObject(std::move(object));
  return *this;
}

IndexAccessBuilder& IndexAccessBuilder::WithIndex(std::unique_ptr<Expr> index_expr) {
  node_->SetIndexExpr(std::move(index_expr));
  return *this;
}

} // namespace ovum::compiler::parser
