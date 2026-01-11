#ifndef PARSER_EXPRSTMT_HPP_
#define PARSER_EXPRSTMT_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class ExprStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr* Expression() const noexcept;
  Expr* MutableExpression() noexcept;
  void SetExpression(std::unique_ptr<Expr> expression);
  std::unique_ptr<Expr> ReleaseExpression();

private:
  std::unique_ptr<Expr> expr_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_EXPRSTMT_HPP_
