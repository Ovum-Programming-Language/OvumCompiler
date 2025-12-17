#include "ContinueStmtBuilder.hpp"

namespace ovum::compiler::parser {

ContinueStmtBuilder& ContinueStmtBuilder::Noop() {
  return *this;
}

} // namespace ovum::compiler::parser