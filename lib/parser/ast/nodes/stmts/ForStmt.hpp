#ifndef FORSTMT_HPP_
#define FORSTMT_HPP_

#include <memory>
#include <string>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

class ForStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  const std::string& IteratorName() const noexcept;
  void SetIteratorName(std::string new_name);

  const Expr* IteratorExpr() const noexcept;
  Expr* MutableIteratorExpr() noexcept;
  void SetIteratorExpr(std::unique_ptr<Expr> expression);
  std::unique_ptr<Expr> ReleaseIteratorExpr();

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> body_block);
  std::unique_ptr<Block> ReleaseBody();

private:
  std::string iter_name_;
  std::unique_ptr<Expr> iter_expr_;
  std::unique_ptr<Block> body_;
};

#endif // FORSTMT_HPP_
