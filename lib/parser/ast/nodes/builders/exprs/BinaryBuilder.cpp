#include "BinaryBuilder.hpp"

namespace ovum::compiler::parser {

BinaryBuilder& BinaryBuilder::WithOp(const IBinaryOpTag& op) {
  node_->SetOp(op);
  return *this;
}

BinaryBuilder& BinaryBuilder::WithLhs(std::unique_ptr<Expr> lhs) {
  node_->SetLhs(std::move(lhs));
  return *this;
}

BinaryBuilder& BinaryBuilder::WithRhs(std::unique_ptr<Expr> rhs) {
  node_->SetRhs(std::move(rhs));
  return *this;
}

}  // namespace ovum::compiler::parser
