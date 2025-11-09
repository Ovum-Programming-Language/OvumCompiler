#include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void StringLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& StringLit::Value() const noexcept {
  return value_;
}

void StringLit::SetValue(std::string new_value) {
  value_ = std::move(new_value);
}

} // namespace ovum::compiler::parser
