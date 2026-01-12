#include <algorithm>

#include "AstNode.hpp"

namespace ovum::compiler::parser {
const SourceSpan& AstNode::Span() const noexcept {
  return span_;
}

void AstNode::SetSpan(SourceSpan span) {
  span_ = std::move(span);
}

void AstNode::SetSpanParts(SourceId id, TokenPosition begin, TokenPosition end) {
  span_ = SourceSpan(std::move(id), begin, end);
}

void AstNode::UnionSpan(const SourceSpan& other) {
  span_ = SourceSpan::Union(span_, other);
}

} // namespace ovum::compiler::parser
