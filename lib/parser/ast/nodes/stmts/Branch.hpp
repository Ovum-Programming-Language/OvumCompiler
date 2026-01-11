#ifndef PARSER_BRANCH_HPP_
#define PARSER_BRANCH_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class Branch {
public:
  Branch(std::unique_ptr<Expr> condition, std::unique_ptr<Block> then_block);
  Branch(const Branch&) = delete;
  Branch& operator=(const Branch&) = delete;
  Branch(Branch&&) noexcept = default;
  Branch& operator=(Branch&&) noexcept = default;
  ~Branch() = default;

  [[nodiscard]] const Expr* Condition() const noexcept;
  [[nodiscard]] Expr* MutableCondition() const noexcept;
  void SetCondition(std::unique_ptr<Expr> expression);
  std::unique_ptr<Expr> ReleaseCondition();

  [[nodiscard]] const Block* Then() const noexcept;
  [[nodiscard]] Block* MutableThen() const noexcept;
  void SetThen(std::unique_ptr<Block> then_body);
  std::unique_ptr<Block> ReleaseThen();

private:
  std::unique_ptr<Expr> condition_;
  std::unique_ptr<Block> then_block_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BRANCH_HPP_
