#ifndef ELVIS_HPP_
#define ELVIS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class Elvis : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

#endif // ELVIS_HPP_
