#include "lib/parser/ast/nodes/stmts/Branch.hpp"

#include <utility>

namespace ovum::compiler::parser {

Branch::Branch(std::unique_ptr<Expr> condition, std::unique_ptr<Block> then_block) :
    condition_(std::move(condition)), then_block_(std::move(then_block)) {
}

const Expr* Branch::Condition() const noexcept {
  return condition_.get();
}

Expr* Branch::MutableCondition() noexcept {
  return condition_.get();
}

void Branch::SetCondition(std::unique_ptr<Expr> expression) {
  condition_ = std::move(expression);
}

std::unique_ptr<Expr> Branch::ReleaseCondition() {
  return std::move(condition_);
}

const Block* Branch::Then() const noexcept {
  return then_block_.get();
}

Block* Branch::MutableThen() noexcept {
  return then_block_.get();
}

void Branch::SetThen(std::unique_ptr<Block> then_body) {
  then_block_ = std::move(then_body);
}

std::unique_ptr<Block> Branch::ReleaseThen() {
  return std::move(then_block_);
}

} // namespace ovum::compiler::parser
