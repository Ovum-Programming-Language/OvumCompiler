#ifndef NULLLIT_HPP_
#define NULLLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class NullLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;
};

#endif // NULLLIT_HPP_
