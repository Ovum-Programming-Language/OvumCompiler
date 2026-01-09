#include "WhileStmtBuilder.hpp"

namespace ovum::compiler::parser {

WhileStmtBuilder& WhileStmtBuilder::WithCondition(std::unique_ptr<Expr> cond) {
  node_->SetCondition(std::move(cond));
  return *this;
}

WhileStmtBuilder& WhileStmtBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser
