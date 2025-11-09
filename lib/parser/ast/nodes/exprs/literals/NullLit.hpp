#ifndef PARSER_NULLLIT_HPP_
#define PARSER_NULLLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class NullLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_NULLLIT_HPP_
