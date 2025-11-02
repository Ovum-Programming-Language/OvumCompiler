#ifndef BRANCH_HPP_
#define BRANCH_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

class Branch Branch {
public:
  Branch(std::unique_ptr<Expr> condition, std::unique_ptr<Block> then_block);
  Branch(const Branch&) = delete;
  Branch& operator=(const Branch&) = delete;
  Branch(Branch&&) noexcept = default;
  Branch& operator=(Branch&&) noexcept = default;
  ~Branch() = default;

  const Expr* Condition() const noexcept;
  Expr* MutableCondition() noexcept;
  void SetCondition(std::unique_ptr<Expr> e);
  std::unique_ptr<Expr> ReleaseCondition();

  const Block* Then() const noexcept;
  Block* MutableThen() noexcept;
  void SetThen(std::unique_ptr<Block> b);
  std::unique_ptr<Block> ReleaseThen();

private:
  std::unique_ptr<Expr> condition_;
  std::unique_ptr<Block> then_block_;
};

#endif // BRANCH_HPP_
