#include "FunctionDeclBuilder.hpp"

namespace ovum::compiler::parser {

FunctionDeclBuilder& FunctionDeclBuilder::WithPure(bool is_pure) {
  node_->SetPure(is_pure);
  return *this;
}

FunctionDeclBuilder& FunctionDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

FunctionDeclBuilder& FunctionDeclBuilder::AddParam(Param parameter) {
  node_->MutableParams().push_back(std::move(parameter));
  return *this;
}

FunctionDeclBuilder& FunctionDeclBuilder::WithReturnType(std::unique_ptr<TypeReference> type) {
  node_->SetReturnType(std::move(type));
  return *this;
}

FunctionDeclBuilder& FunctionDeclBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser
