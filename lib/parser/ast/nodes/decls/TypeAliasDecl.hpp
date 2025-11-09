#ifndef TYPEALIASDECL_HPP_
#define TYPEALIASDECL_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

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

#endif // TYPEALIASDECL_HPP_
