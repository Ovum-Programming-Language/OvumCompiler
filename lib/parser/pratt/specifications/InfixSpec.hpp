#ifndef INFIXSPEC_HPP_
#define INFIXSPEC_HPP_

#include <functional>

#include "lib/lexer/tokens/Token.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IBinaryOpTag.hpp"

class InfixSpec {
public:
  InfixSpec(int lbp, int rbp, bool right_associative, IBinaryOpTag* tag, bool is_elvis);
  // TODO: implement getters and setters
private:
  std::function<bool(const Token&)> match_;
  int lbp_ = 0;
  int rbp_ = 0;
  bool right_associative_ = false;
  const IBinaryOpTag* tag_ = nullptr;
  bool is_elvis_ = false;
};

#endif // INFIXSPEC_HPP_
