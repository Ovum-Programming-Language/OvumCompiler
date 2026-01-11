#include "Block.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void Block::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

std::vector<std::unique_ptr<Stmt>>& Block::GetStatements() {
  return stmts_;
}

const std::vector<std::unique_ptr<Stmt>>& Block::GetStatements() const {
  return stmts_;
}

void Block::Append(std::unique_ptr<Stmt> statement) {
  stmts_.emplace_back(std::move(statement));
}

void Block::Insert(std::size_t index, std::unique_ptr<Stmt> statement) {
  if (index >= stmts_.size()) {
    stmts_.emplace_back(std::move(statement));
  } else {
    stmts_.insert(stmts_.begin() + static_cast<std::ptrdiff_t>(index), std::move(statement));
  }
}

std::unique_ptr<Stmt> Block::ReleaseAt(std::size_t index) {
  if (index >= stmts_.size()) {
    return nullptr;
  }

  auto old_stmt = std::move(stmts_[index]);
  stmts_.erase(stmts_.begin() + static_cast<std::ptrdiff_t>(index));
  return old_stmt;
}

void Block::Clear() noexcept {
  stmts_.clear();
}

size_t Block::Size() const noexcept {
  return stmts_.size();
}

} // namespace ovum::compiler::parser
