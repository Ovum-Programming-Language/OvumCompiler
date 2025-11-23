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

} // namespace ovum::compiler::parser
