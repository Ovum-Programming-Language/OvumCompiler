#ifndef WHILESTMT_HPP_
#define WHILESTMT_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class WhileStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> cond_;
  std::unique_ptr<Block> body_;
};

#endif // WHILESTMT_HPP_
