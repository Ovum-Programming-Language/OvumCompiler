#ifndef INTLIT_HPP_
#define INTLIT_HPP_
#include "lib/parser/ast/nodes/base/Expr.hpp"

class IntLit : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  long long value = 0;
};
#endif // INTLIT_HPP_
