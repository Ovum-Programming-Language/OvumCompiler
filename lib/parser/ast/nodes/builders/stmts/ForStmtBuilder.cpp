#include "ForStmtBuilder.hpp"

namespace ovum::compiler::parser {

ForStmtBuilder& ForStmtBuilder::WithIteratorName(std::string name) {
  node_->SetIteratorName(std::move(name));
  return *this;
}

ForStmtBuilder& ForStmtBuilder::WithIteratorExpr(std::unique_ptr<Expr> expr) {
  node_->SetIteratorExpr(std::move(expr));
  return *this;
}

ForStmtBuilder& ForStmtBuilder::WithBody(std::unique_ptr<Block> body) {
  node_->SetBody(std::move(body));
  return *this;
}

} // namespace ovum::compiler::parser