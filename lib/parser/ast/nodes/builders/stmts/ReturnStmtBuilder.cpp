#include "ReturnStmtBuilder.hpp"

namespace ovum::compiler::parser {

ReturnStmtBuilder& ReturnStmtBuilder::WithValue(std::unique_ptr<Expr> value) {
  node_->SetValue(std::move(value));
  return *this;
}

} // namespace ovum::compiler::parser
