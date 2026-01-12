#include "ElvisBuilder.hpp"

namespace ovum::compiler::parser {

ElvisBuilder& ElvisBuilder::WithLhs(std::unique_ptr<Expr> lhs) {
  node_->SetLhs(std::move(lhs));
  return *this;
}

ElvisBuilder& ElvisBuilder::WithRhs(std::unique_ptr<Expr> rhs) {
  node_->SetRhs(std::move(rhs));
  return *this;
}

} // namespace ovum::compiler::parser
