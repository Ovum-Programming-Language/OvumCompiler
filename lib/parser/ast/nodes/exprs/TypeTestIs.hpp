#ifndef TYPETESTIS_HPP_
#define TYPETESTIS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class TypeTestIs : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> expr;
  TypeReference type;
};

#endif // TYPETESTIS_HPP_
