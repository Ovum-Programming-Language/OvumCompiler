#include "BoolLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void BoolLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool BoolLit::Value() const noexcept {
  return value_;
}

void BoolLit::SetValue(bool new_value) noexcept {
  value_ = new_value;
}

} // namespace ovum::compiler::parser
