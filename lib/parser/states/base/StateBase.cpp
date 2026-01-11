#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/SourceId.hpp"

namespace ovum::compiler::parser {

SourceSpan StateBase::SpanFrom(const Token& token) {
  const auto& pos = token.GetPosition();
  return {SourceId{}, pos, pos};
}

SourceSpan StateBase::Union(const SourceSpan& lhs, const SourceSpan& rhs) {
  return SourceSpan::Union(lhs, rhs);
}

} // namespace ovum::compiler::parser
