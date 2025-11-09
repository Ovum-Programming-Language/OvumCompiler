#ifndef WHILESTMT_HPP_
#define WHILESTMT_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class WhileStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  const Expr* Condition() const noexcept;
  Expr* MutableCondition() noexcept;
  void SetCondition(std::unique_ptr<Expr> condition_expr);
  std::unique_ptr<Expr> ReleaseCondition();

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> body_block);
  std::unique_ptr<Block> ReleaseBody();

private:
  std::unique_ptr<Expr> cond_;
  std::unique_ptr<Block> body_;
};

#endif // WHILESTMT_HPP_
