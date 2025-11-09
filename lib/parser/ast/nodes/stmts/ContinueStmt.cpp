#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void ContinueStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}
