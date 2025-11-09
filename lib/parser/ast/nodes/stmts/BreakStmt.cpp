#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void BreakStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

} // namespace ovum::compiler::parser
