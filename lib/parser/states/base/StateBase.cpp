#include "lib/parser/states/base/StateBase.hpp"
#include "lib/parser/tokens/SourceId.hpp"

namespace ovum::compiler::parser {

SourceSpan StateBase::SpanFrom(const Token& token) {
  const auto& pos = token.GetPosition();
  return SourceSpan(SourceId{}, pos, pos);
}

SourceSpan StateBase::Union(const SourceSpan& left, const SourceSpan& right) {
  return SourceSpan::Union(left, right);
}

} // namespace ovum::compiler::parser
