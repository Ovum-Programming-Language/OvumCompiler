#include "ThisExpr.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void ThisExpr::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

} // namespace ovum::compiler::parser
