#include "ThisExprBuilder.hpp"

namespace ovum::compiler::parser {

ThisExprBuilder& ThisExprBuilder::Noop() {
  return *this;
}

} // namespace ovum::compiler::parser
