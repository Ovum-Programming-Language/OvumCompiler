#ifndef PARSER_SOURCESPAN_HPP_
#define PARSER_SOURCESPAN_HPP_

#include <tokens/TokenPosition.hpp>

#include "SourceId.hpp"

namespace ovum::compiler::parser {

class SourceSpan {
public:
  SourceSpan() = default;
  SourceSpan(SourceId id, TokenPosition start, TokenPosition end);

  [[nodiscard]] const SourceId& GetSourceId() const noexcept;
  [[nodiscard]] TokenPosition GetStart() const noexcept;
  [[nodiscard]] TokenPosition GetEnd() const noexcept;

  [[nodiscard]] bool IsValid() const noexcept;
  void Normalize() noexcept;
  [[nodiscard]] static SourceSpan SinglePoint(SourceId id, TokenPosition p);

  [[nodiscard]] static SourceSpan Union(const SourceSpan& a, const SourceSpan& b);

private:
  SourceId id_;
  TokenPosition begin_{};
  TokenPosition end_{};
};

} // namespace ovum::compiler::parser

#endif // PARSER_SOURCESPAN_HPP_
