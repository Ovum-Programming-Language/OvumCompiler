#include "NullLitBuilder.hpp"

namespace ovum::compiler::parser {

NullLitBuilder& NullLitBuilder::Noop() {
  return *this;
}

} // namespace ovum::compiler::parser
