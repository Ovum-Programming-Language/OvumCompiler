#include "AssignBuilder.hpp"

namespace ovum::compiler::parser {

AssignBuilder& AssignBuilder::WithKind(const IAssignOpTag& kind) {
  node_->SetKind(kind);
  return *this;
}

AssignBuilder& AssignBuilder::WithTarget(std::unique_ptr<Expr> target) {
  node_->SetTarget(std::move(target));
  return *this;
}

AssignBuilder& AssignBuilder::WithValue(std::unique_ptr<Expr> value) {
  node_->SetValue(std::move(value));
  return *this;
}

} // namespace ovum::compiler::parser
