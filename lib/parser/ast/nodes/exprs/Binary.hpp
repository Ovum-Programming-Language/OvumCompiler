#ifndef BINARY_HPP_
#define BINARY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/OpTags.hpp"

class Binary : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  const IBinaryOpTag* op_ = &OpTags::Add();
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

#endif // BINARY_HPP_
