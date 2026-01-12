#include "FieldAccessBuilder.hpp"

namespace ovum::compiler::parser {

FieldAccessBuilder& FieldAccessBuilder::WithObject(std::unique_ptr<Expr> object) {
  node_->SetObject(std::move(object));
  return *this;
}

FieldAccessBuilder& FieldAccessBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

} // namespace ovum::compiler::parser
