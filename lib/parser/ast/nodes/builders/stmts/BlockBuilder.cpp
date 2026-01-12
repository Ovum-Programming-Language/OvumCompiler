#include "BlockBuilder.hpp"

namespace ovum::compiler::parser {

BlockBuilder& BlockBuilder::Append(std::unique_ptr<Stmt> statement) {
  node_->Append(std::move(statement));
  return *this;
}

BlockBuilder& BlockBuilder::WithStatements(std::vector<std::unique_ptr<Stmt>> statements) {
  for (auto& stmt : statements) {
    node_->Append(std::move(stmt));
  }
  return *this;
}

} // namespace ovum::compiler::parser
