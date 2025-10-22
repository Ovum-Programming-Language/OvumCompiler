#ifndef POSTFIXSPEC_HPP_
#define POSTFIXSPEC_HPP_

#include <functional>

#include "lib/lexer/tokens/Token.hpp"

class PostfixSpec {
public:
  // TODO: implements getters and setters
private:
  std::function<bool(const Token&)> match_;
  int bp = 0;
  bool keyword = false;
};

#endif // POSTFIXSPEC_HPP_
