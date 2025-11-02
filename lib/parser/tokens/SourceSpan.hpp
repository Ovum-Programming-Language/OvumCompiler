#ifndef SOURCESPAN_HPP_
#define SOURCESPAN_HPP_

#include "SourceId.hpp"

#include "lib/lexer/tokens/TokenPosition.hpp"

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

#endif // SOURCESPAN_HPP_
