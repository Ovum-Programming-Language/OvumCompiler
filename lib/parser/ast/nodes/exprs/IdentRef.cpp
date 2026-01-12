#include "IdentRef.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void IdentRef::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& IdentRef::Name() const noexcept {
  return name_;
}

void IdentRef::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

} // namespace ovum::compiler::parser
