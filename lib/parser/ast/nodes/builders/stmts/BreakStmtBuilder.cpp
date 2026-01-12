#include "BreakStmtBuilder.hpp"

namespace ovum::compiler::parser {

BreakStmtBuilder& BreakStmtBuilder::Noop() {
  return *this;
}

} // namespace ovum::compiler::parser
