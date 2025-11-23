#include "SafeCallBuilder.hpp"

namespace ovum::compiler::parser {

SafeCallBuilder& SafeCallBuilder::WithObject(std::unique_ptr<Expr> object) {
  node_->SetObject(std::move(object));
  return *this;
}

SafeCallBuilder& SafeCallBuilder::WithMethod(std::string method) {
  node_->SetMethod(std::move(method));
  return *this;
}

SafeCallBuilder& SafeCallBuilder::AddArg(std::unique_ptr<Expr> argument) {
  node_->AddArg(std::move(argument));
  return *this;
}

SafeCallBuilder& SafeCallBuilder::ClearArgs() {
  node_->ClearArgs();
  return *this;
}

} // namespace ovum::compiler::parser
