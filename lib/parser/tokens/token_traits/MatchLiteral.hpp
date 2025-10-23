#ifndef MATCHLITERAL_HPP_
#define MATCHLITERAL_HPP_

#include "ITokenMatcher.hpp"
#include "lib/lexer/tokens/Token.hpp"

class MatchLiteral : public ITokenMatcher {
public:
  bool Match(const Token& tok) const override;
};

#endif // MATCHLITERAL_HPP_
