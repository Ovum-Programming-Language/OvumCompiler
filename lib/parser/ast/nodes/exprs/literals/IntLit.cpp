#include "IntLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void IntLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

int64_t IntLit::Value() const noexcept {
  return value;
}

void IntLit::SetValue(int64_t new_value) noexcept {
  value = new_value;
}

} // namespace ovum::compiler::parser
