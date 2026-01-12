#include "SourceSpan.hpp"

#include <utility>

namespace ovum::compiler::parser {

SourceSpan::SourceSpan(SourceId id, const TokenPosition start, const TokenPosition end) :
    id_(std::move(id)), begin_(start), end_(end) {
  Normalize();
}

const SourceId& SourceSpan::GetSourceId() const noexcept {
  return id_;
}
TokenPosition SourceSpan::GetStart() const noexcept {
  return begin_;
}
TokenPosition SourceSpan::GetEnd() const noexcept {
  return end_;
}

bool SourceSpan::IsValid() const noexcept {
  return id_.IsValid();
}

void SourceSpan::Normalize() noexcept {
  if (end_ < begin_) {
    std::swap(begin_, end_);
  }
}

SourceSpan SourceSpan::SinglePoint(SourceId id, TokenPosition point) {
  return {std::move(id), point, point};
}

SourceSpan SourceSpan::Union(const SourceSpan& a, const SourceSpan& b) {
  if (!a.IsValid()) {
    return b;
  }

  if (!b.IsValid()) {
    return a;
  }

  if (a.id_ != b.id_) {
    return a;
  }
  const TokenPosition start = a.begin_ < b.begin_ ? a.begin_ : b.begin_;
  const TokenPosition end = a.end_ < b.end_ ? b.end_ : a.end_;
  return {a.id_, start, end};
}

} // namespace ovum::compiler::parser
