#ifndef PARSER_INFIXSPEC_HPP_
#define PARSER_INFIXSPEC_HPP_

#include <functional>

#include <tokens/Token.hpp>

#include "lib/parser/ast/nodes/exprs/tags/IBinaryOpTag.hpp"

namespace ovum::compiler::parser {

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

} // namespace ovum::compiler::parser

#endif // PARSER_INFIXSPEC_HPP_
