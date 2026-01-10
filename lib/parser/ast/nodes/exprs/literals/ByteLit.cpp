#include "ByteLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void ByteLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

uint8_t ByteLit::Value() const noexcept {
  return value;
}

void ByteLit::SetValue(uint8_t new_value) noexcept {
  value = new_value;
}

} // namespace ovum::compiler::parser

