#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

void IfStmt::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::vector<Branch>& IfStmt::Branches() const noexcept {
  return branches_;
}

std::vector<Branch>& IfStmt::MutableBranches() noexcept {
  return branches_;
}

void IfStmt::AddBranch(Branch branch_value) {
  branches_.emplace_back(std::move(branch_value));
}

void IfStmt::EmplaceBranch(std::unique_ptr<Expr> cond, std::unique_ptr<Block> then_blk) {
  branches_.emplace_back(std::move(cond), std::move(then_blk));
}

const Block* IfStmt::ElseBlock() const noexcept {
  return else_block_.get();
}

Block* IfStmt::MutableElseBlock() noexcept {
  return else_block_.get();
}

void IfStmt::SetElseBlock(std::unique_ptr<Block> else_body) {
  else_block_ = std::move(else_body);
}

std::unique_ptr<Block> IfStmt::ReleaseElseBlock() {
  return std::move(else_block_);
}

bool IfStmt::HasElse() const noexcept {
  return static_cast<bool>(else_block_);
}

bool IfStmt::Empty() const noexcept {
  return branches_.empty() && !else_block_;
}
