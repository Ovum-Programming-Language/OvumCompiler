#ifndef PARSER_TYPEALIASDECL_HPP_
#define PARSER_TYPEALIASDECL_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class TypeAliasDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const TypeReference& AliasedType() const noexcept;
  TypeReference& MutableAliasedType() noexcept;
  void SetAliasedType(TypeReference type);

private:
  std::string name_;
  TypeReference aliased_type_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_TYPEALIASDECL_HPP_
