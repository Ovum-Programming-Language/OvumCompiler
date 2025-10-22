#ifndef FLOATLIT_HPP_
#define FLOATLIT_HPP_
#include "lib/parser/ast/nodes/base/Expr.hpp"

class FloatLit : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  long double value = 0.0L;
};

#endif // FLOATLIT_HPP_
