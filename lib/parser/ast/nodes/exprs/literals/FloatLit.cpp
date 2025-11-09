#include "lib/parser/ast/nodes/exprs/literals/FloatLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void FloatLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

double FloatLit::Value() const noexcept {
  return value;
}

void FloatLit::SetValue(double new_value) noexcept {
  value = new_value;
}
