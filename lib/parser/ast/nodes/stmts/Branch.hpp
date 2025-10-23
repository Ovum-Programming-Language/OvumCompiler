#ifndef BRANCH_HPP_
#define BRANCH_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

class Branch {
public:
  Branch(std::unique_ptr<Expr> expr, std::unique_ptr<Block> block);

  [[nodiscard]] std::unique_ptr<Block> GetBlock();

  void AddBlock(std::unique_ptr<Block> block);

  [[nodiscard]] std::unique_ptr<Expr> GetExpr();

  void AddExpr(std::unique_ptr<Expr> expr);

private:
  std::unique_ptr<Expr> condition_;
  std::unique_ptr<Block> then_block_;
};

#endif // BRANCH_HPP_
