#ifndef ITOKENMATCHER_HPP_
#define ITOKENMATCHER_HPP_

#include "lib/lexer/tokens/Token.hpp"

class ITokenMatcher {
public:
  virtual ~ITokenMatcher() = default;

  virtual bool Match(const Token& t) const = 0;
};

#endif // ITOKENMATCHER_HPP_
