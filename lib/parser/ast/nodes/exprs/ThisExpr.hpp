#ifndef PARSER_THIS_EXPR_HPP_
#define PARSER_THIS_EXPR_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class ThisExpr : public Expr {
public:
  void Accept(AstVisitor& visitor) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_THIS_EXPR_HPP_
