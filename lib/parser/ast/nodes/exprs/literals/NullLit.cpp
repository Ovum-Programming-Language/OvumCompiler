#include "lib/parser/ast/nodes/exprs/literals/NullLit.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void NullLit::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}
