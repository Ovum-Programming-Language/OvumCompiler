#ifndef POSTFIXSPEC_HPP_
#define POSTFIXSPEC_HPP_

#include <functional>

#include "lib/lexer/tokens/Token.hpp"

class PostfixSpec {
public:
  PostfixSpec(std::function<bool(const Token&)> match, int bp, bool keyword);
  // TODO: implements getters and setters
private:
  std::function<bool(const Token&)> match_;
  int bp = 0;
  bool keyword = false;
};

#endif // POSTFIXSPEC_HPP_
