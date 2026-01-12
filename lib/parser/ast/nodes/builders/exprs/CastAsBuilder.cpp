#include "CastAsBuilder.hpp"

namespace ovum::compiler::parser {

CastAsBuilder& CastAsBuilder::WithExpr(std::unique_ptr<Expr> expression) {
  node_->SetExpression(std::move(expression));
  return *this;
}

CastAsBuilder& CastAsBuilder::WithType(TypeReference type) {
  node_->SetType(std::move(type));
  return *this;
}

} // namespace ovum::compiler::parser
