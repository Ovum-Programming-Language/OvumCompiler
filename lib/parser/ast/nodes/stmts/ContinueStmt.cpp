#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void ContinueStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

} // namespace ovum::compiler::parser
