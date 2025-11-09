#ifndef BINARY_HPP_
#define BINARY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/IBinaryOpTag.hpp"
#include "tags/OpTags.hpp"

class Binary : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const IBinaryOpTag& Op() const noexcept;
  void SetOp(const IBinaryOpTag& new_op) noexcept;

  const Expr& Lhs() const noexcept;
  Expr& MutableLhs() noexcept;
  void SetLhs(std::unique_ptr<Expr> new_lhs);
  std::unique_ptr<Expr> ReplaceLhs(std::unique_ptr<Expr> new_lhs);

  const Expr& Rhs() const noexcept;
  Expr& MutableRhs() noexcept;
  void SetRhs(std::unique_ptr<Expr> new_rhs);
  std::unique_ptr<Expr> ReplaceRhs(std::unique_ptr<Expr> new_rhs);

private:
  const IBinaryOpTag* op_ = &OpTags::Add();
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

#endif // BINARY_HPP_
