#ifndef IFSTMT_HPP_
#define IFSTMT_HPP_

#include <memory>
#include <vector>

#include "Block.hpp"
#include "Branch.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class IfStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  const std::vector<Branch>& Branches() const noexcept;
  std::vector<Branch>& MutableBranches() noexcept;
  void AddBranch(Branch branch_value);
  void EmplaceBranch(std::unique_ptr<Expr> cond, std::unique_ptr<Block> then_blk);

  const Block* ElseBlock() const noexcept;
  Block* MutableElseBlock() noexcept;
  void SetElseBlock(std::unique_ptr<Block> else_body);
  std::unique_ptr<Block> ReleaseElseBlock();

  bool HasElse() const noexcept;
  bool Empty() const noexcept;

private:
  std::vector<Branch> branches_;
  std::unique_ptr<Block> else_block_;
};

#endif // IFSTMT_HPP_
