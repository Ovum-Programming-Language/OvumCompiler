#include "NullLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void NullLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

} // namespace ovum::compiler::parser
