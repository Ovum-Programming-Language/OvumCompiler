#include "IfStmtBuilder.hpp"

namespace ovum::compiler::parser {

IfStmtBuilder& IfStmtBuilder::AddBranch(Branch branch) {
  node_->AddBranch(std::move(branch));
  return *this;
}

IfStmtBuilder& IfStmtBuilder::WithBranches(std::vector<Branch> branches) {
  for (auto& branch : branches) {
    node_->AddBranch(std::move(branch));
  }
  return *this;
}

IfStmtBuilder& IfStmtBuilder::WithElse(std::unique_ptr<Block> else_block) {
  node_->SetElseBlock(std::move(else_block));
  return *this;
}

} // namespace ovum::compiler::parser
