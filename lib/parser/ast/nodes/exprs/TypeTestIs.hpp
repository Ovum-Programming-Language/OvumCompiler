#ifndef PARSER_TYPETESTIS_HPP_
#define PARSER_TYPETESTIS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class TypeTestIs : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& Expression() const noexcept;
  [[nodiscard]] Expr& MutableExpression() const noexcept;
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

#endif // PARSER_TYPETESTIS_HPP_
