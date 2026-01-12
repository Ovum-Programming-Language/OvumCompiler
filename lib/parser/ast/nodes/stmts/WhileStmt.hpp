#ifndef PARSER_WHILESTMT_HPP_
#define PARSER_WHILESTMT_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class WhileStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr* Condition() const noexcept;
  [[nodiscard]] Expr* MutableCondition() const noexcept;
  void SetCondition(std::unique_ptr<Expr> condition_expr);
  std::unique_ptr<Expr> ReleaseCondition();

  [[nodiscard]] const Block* Body() const noexcept;
  [[nodiscard]] Block* MutableBody() const noexcept;
  void SetBody(std::unique_ptr<Block> body_block);
  std::unique_ptr<Block> ReleaseBody();

private:
  std::unique_ptr<Expr> cond_;
  std::unique_ptr<Block> body_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_WHILESTMT_HPP_
