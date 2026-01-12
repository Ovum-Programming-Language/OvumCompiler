#include "UnaryBuilder.hpp"

namespace ovum::compiler::parser {

UnaryBuilder& UnaryBuilder::WithOp(const IUnaryOpTag& op) {
  node_->SetOp(op);
  return *this;
}

UnaryBuilder& UnaryBuilder::WithOperand(std::unique_ptr<Expr> operand) {
  node_->SetOperand(std::move(operand));
  return *this;
}

} // namespace ovum::compiler::parser
