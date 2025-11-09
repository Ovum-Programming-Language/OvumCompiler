#include "lib/parser/ast/nodes/exprs/IdentRef.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void IdentRef::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& IdentRef::Name() const noexcept {
  return name_;
}

void IdentRef::SetName(std::string new_name) {
  name_ = std::move(new_name);
}
