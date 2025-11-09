#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void BreakStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}
