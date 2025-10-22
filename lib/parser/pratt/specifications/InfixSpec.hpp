#ifndef INFIXSPEC_HPP_
#define INFIXSPEC_HPP_

#include <functional>

#include "lib/lexer/tokens/Token.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IBinaryOpTag.hpp"

class InfixSpec {
public:
  // TODO: implement getters and setters
private:
  std::function<bool(const Token&)> match_;
  int lbp = 0;
  int rbp = 0;
  bool right_associative = false;
  const IBinaryOpTag* tag_ = nullptr;
  bool is_elvis = false;
};

#endif // INFIXSPEC_HPP_
