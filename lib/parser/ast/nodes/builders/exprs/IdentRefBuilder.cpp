#include "IdentRefBuilder.hpp"

namespace ovum::compiler::parser {

IdentRefBuilder& IdentRefBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

} // namespace ovum::compiler::parser
