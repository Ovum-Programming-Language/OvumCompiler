#include "CallDeclBuilder.hpp"

namespace ovum::compiler::parser {

CallDeclBuilder& CallDeclBuilder::WithPublic(bool is_public) {
  node_->SetPublic(is_public);
  return *this;
}

CallDeclBuilder& CallDeclBuilder::WithIsPublic(bool is_public) {
  return WithPublic(is_public);
}

CallDeclBuilder& CallDeclBuilder::AddParam(Param parameter) {
  node_->MutableParams().push_back(std::move(parameter));
  return *this;
}

CallDeclBuilder& CallDeclBuilder::WithParams(std::vector<Param> params) {
  for (auto& param : params) {
    node_->MutableParams().push_back(std::move(param));
  }
  return *this;
}

CallDeclBuilder& CallDeclBuilder::WithReturnType(std::unique_ptr<TypeReference> type) {
  node_->SetReturnType(std::move(type));
  return *this;
}

CallDeclBuilder& CallDeclBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser
