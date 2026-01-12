#include "CallExprBuilder.hpp"

namespace ovum::compiler::parser {

CallExprBuilder& CallExprBuilder::WithCallee(std::unique_ptr<Expr> callee) {
  node_->SetCallee(std::move(callee));
  return *this;
}

CallExprBuilder& CallExprBuilder::AddArg(std::unique_ptr<Expr> argument) {
  node_->AddArg(std::move(argument));
  return *this;
}

CallExprBuilder& CallExprBuilder::WithArgs(std::vector<std::unique_ptr<Expr>> args) {
  for (auto& arg : args) {
    node_->AddArg(std::move(arg));
  }
  return *this;
}

} // namespace ovum::compiler::parser
