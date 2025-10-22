#ifndef IFSTMT_HPP_
#define IFSTMT_HPP_

#include <memory>
#include <vector>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class IfStmt : public Stmt {
public:
  struct Branch {
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Block> then_block;
  };

  void Accept(AstVisitor& v) override;

private:
  std::vector<Branch> branches_; // if + else-if*
  std::unique_ptr<Block> else_block_;
};

#endif // IFSTMT_HPP_
