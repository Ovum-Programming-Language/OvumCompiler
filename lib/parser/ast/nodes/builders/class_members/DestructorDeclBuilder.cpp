#include "DestructorDeclBuilder.hpp"

namespace ovum::compiler::parser {

DestructorDeclBuilder& DestructorDeclBuilder::WithPublic(bool is_public) {
  node_->SetPublic(is_public);
  return *this;
}

DestructorDeclBuilder& DestructorDeclBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

DestructorDeclBuilder& DestructorDeclBuilder::WithIsPublic(bool is_public) {
  return WithPublic(is_public);
}

} // namespace ovum::compiler::parser
