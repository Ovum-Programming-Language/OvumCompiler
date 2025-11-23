#include "TypeAliasDeclBuilder.hpp"

namespace ovum::compiler::parser {

TypeAliasDeclBuilder& TypeAliasDeclBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

TypeAliasDeclBuilder& TypeAliasDeclBuilder::WithAliasedType(TypeReference type) {
  node_->SetAliasedType(std::move(type));
  return *this;
}

} // namespace ovum::compiler::parser
