#include "lib/parser/ast/nodes/exprs/literals/CharLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void CharLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

char CharLit::Value() const noexcept {
  return value_;
}

void CharLit::SetValue(char new_value) noexcept {
  value_ = new_value;
}

} // namespace ovum::compiler::parser
