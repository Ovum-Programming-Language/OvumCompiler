#ifndef CASTAS_HPP_
#define CASTAS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class CastAs : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const Expr& Expression() const noexcept;
  Expr& MutableExpression() noexcept;
  void SetExpression(std::unique_ptr<Expr> new_expression);
  std::unique_ptr<Expr> ReplaceExpression(std::unique_ptr<Expr> new_expression);

  const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference new_type);

private:
  std::unique_ptr<Expr> expr_;
  TypeReference type_;
};

#endif // CASTAS_HPP_
