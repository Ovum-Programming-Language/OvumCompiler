#include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void BoolLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool BoolLit::Value() const noexcept {
  return value;
}

void BoolLit::SetValue(bool new_value) noexcept {
  value = new_value;
}

} // namespace ovum::compiler::parser
