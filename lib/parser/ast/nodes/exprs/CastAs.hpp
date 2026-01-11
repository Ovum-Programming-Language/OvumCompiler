#ifndef PARSER_CASTAS_HPP_
#define PARSER_CASTAS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class CastAs : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& Expression() const noexcept;
  Expr& MutableExpression() noexcept;
  void SetExpression(std::unique_ptr<Expr> new_expression);
  std::unique_ptr<Expr> ReplaceExpression(std::unique_ptr<Expr> new_expression);

  [[nodiscard]] const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference new_type);

private:
  std::unique_ptr<Expr> expr_;
  TypeReference type_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CASTAS_HPP_
