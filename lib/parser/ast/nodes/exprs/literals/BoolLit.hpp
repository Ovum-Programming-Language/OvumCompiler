#ifndef BOOLLIT_HPP_
#define BOOLLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class BoolLit : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  bool value = false;
};

#endif // BOOLLIT_HPP_
