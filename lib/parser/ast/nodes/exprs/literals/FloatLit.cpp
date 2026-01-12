#include "FloatLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void FloatLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

double FloatLit::Value() const noexcept {
  return value_;
}

void FloatLit::SetValue(double new_value) noexcept {
  value_ = new_value;
}

} // namespace ovum::compiler::parser
