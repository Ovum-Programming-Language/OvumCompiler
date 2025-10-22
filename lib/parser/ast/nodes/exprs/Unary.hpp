#ifndef UNARY_HPP_
#define UNARY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/IUnaryOpTag.hpp"
#include "tags/OpTags.hpp"

class Unary : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  const IUnaryOpTag* op_ = &OpTags::Neg();
  std::unique_ptr<Expr> operand_;
};

#endif // UNARY_HPP_
