#include "TypeAliasDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void TypeAliasDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& TypeAliasDecl::Name() const noexcept {
  return name_;
}

void TypeAliasDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const TypeReference& TypeAliasDecl::AliasedType() const noexcept {
  return aliased_type_;
}

TypeReference& TypeAliasDecl::MutableAliasedType() noexcept {
  return aliased_type_;
}

void TypeAliasDecl::SetAliasedType(TypeReference type) {
  aliased_type_ = std::move(type);
}

} // namespace ovum::compiler::parser
