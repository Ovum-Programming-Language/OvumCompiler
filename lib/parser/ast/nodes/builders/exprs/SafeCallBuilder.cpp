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

SafeCallBuilder& SafeCallBuilder::WithArgs(std::vector<std::unique_ptr<Expr>> args) {
  node_->ClearArgs();
  for (auto& arg : args) {
    node_->AddArg(std::move(arg));
  }
  return *this;
}

SafeCallBuilder& SafeCallBuilder::WithInferredType(std::optional<TypeReference> inferred_type) {
  if (inferred_type.has_value()) {
    node_->SetInferredType(inferred_type.value());
  }
  return *this;
}

SafeCallBuilder& SafeCallBuilder::ClearArgs() {
  node_->ClearArgs();
  return *this;
}

} // namespace ovum::compiler::parser
