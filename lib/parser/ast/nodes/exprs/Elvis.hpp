#ifndef PARSER_ELVIS_HPP_
#define PARSER_ELVIS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class Elvis : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const Expr& Lhs() const noexcept;
  Expr& MutableLhs() noexcept;
  void SetLhs(std::unique_ptr<Expr> new_lhs);
  std::unique_ptr<Expr> ReplaceLhs(std::unique_ptr<Expr> new_lhs);

  const Expr& Rhs() const noexcept;
  Expr& MutableRhs() noexcept;
  void SetRhs(std::unique_ptr<Expr> new_rhs);
  std::unique_ptr<Expr> ReplaceRhs(std::unique_ptr<Expr> new_rhs);

private:
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ELVIS_HPP_
