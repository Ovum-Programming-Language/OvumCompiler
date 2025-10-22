#ifndef TYPEALIASDECL_HPP_
#define TYPEALIASDECL_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

class TypeAliasDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  std::string name_;
  TypeReference aliased_type_;
};

#endif // TYPEALIASDECL_HPP_
