#ifndef PARSER_FORSTMT_HPP_
#define PARSER_FORSTMT_HPP_

#include <memory>
#include <string>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class ForStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const std::string& IteratorName() const noexcept;
  void SetIteratorName(std::string new_name);

  [[nodiscard]] const Expr* IteratorExpr() const noexcept;
  [[nodiscard]] Expr* MutableIteratorExpr() const noexcept;
  void SetIteratorExpr(std::unique_ptr<Expr> expression);
  std::unique_ptr<Expr> ReleaseIteratorExpr();

  [[nodiscard]] const Block* Body() const noexcept;
  [[nodiscard]] Block* MutableBody() const noexcept;
  void SetBody(std::unique_ptr<Block> body_block);
  std::unique_ptr<Block> ReleaseBody();

private:
  std::string iter_name_;
  std::unique_ptr<Expr> iter_expr_;
  std::unique_ptr<Block> body_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_FORSTMT_HPP_
