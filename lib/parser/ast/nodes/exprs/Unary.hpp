#ifndef UNARY_HPP_
#define UNARY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/IUnaryOpTag.hpp"
#include "tags/OpTags.hpp"

class Unary : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const IUnaryOpTag& Op() const noexcept;
  void SetOp(const IUnaryOpTag& new_op) noexcept;

  const Expr& Operand() const noexcept;
  Expr& MutableOperand() noexcept;
  void SetOperand(std::unique_ptr<Expr> new_operand);
  std::unique_ptr<Expr> ReplaceOperand(std::unique_ptr<Expr> new_operand);

private:
  const IUnaryOpTag* op_ = &OpTags::Neg();
  std::unique_ptr<Expr> operand_;
};

#endif // UNARY_HPP_
