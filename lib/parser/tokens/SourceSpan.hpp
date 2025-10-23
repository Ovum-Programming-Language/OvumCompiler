#ifndef SOURCESPAN_HPP_
#define SOURCESPAN_HPP_

#include "lib/lexer/tokens/TokenPosition.hpp"

class SourceSpan {
public:
  SourceSpan(TokenPosition start, TokenPosition end);

  [[nodiscard]] TokenPosition GetStart() const;

  [[nodiscard]] TokenPosition GetEnd() const;

private:
  TokenPosition begin_;
  TokenPosition end_;
};

#endif // SOURCESPAN_HPP_
