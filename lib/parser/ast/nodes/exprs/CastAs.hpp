#ifndef CASTAS_HPP_
#define CASTAS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class CastAs : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> expr_;
  TypeReference type_;
};

#endif // CASTAS_HPP_
