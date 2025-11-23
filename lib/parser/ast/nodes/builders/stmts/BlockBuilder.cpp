#include "BlockBuilder.hpp"

namespace ovum::compiler::parser {

BlockBuilder& BlockBuilder::Append(std::unique_ptr<Stmt> statement) {
  node_->Append(std::move(statement));
  return *this;
}

} // namespace ovum::compiler::parser
