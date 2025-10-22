#ifndef MATCHIDENTIFIER_HPP_
#define MATCHIDENTIFIER_HPP_

#include "ITokenMatcher.hpp"
#include "lib/lexer/tokens/Token.hpp"

class MatchIdentifier : public ITokenMatcher {
public:
  bool Match(const Token& tok) const override;
};

#endif // MATCHIDENTIFIER_HPP_
