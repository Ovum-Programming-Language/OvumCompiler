#ifndef PARSER_ASSIGN_HPP_
#define PARSER_ASSIGN_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/IAssignOpTag.hpp"
#include "tags/optags.hpp"

namespace ovum::compiler::parser {

class Assign : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const IAssignOpTag& Kind() const noexcept;
  void SetKind(const IAssignOpTag& new_kind) noexcept;

  [[nodiscard]] const Expr& Target() const noexcept;
  Expr& MutableTarget() noexcept;
  void SetTarget(std::unique_ptr<Expr> new_target);
  std::unique_ptr<Expr> ReplaceTarget(std::unique_ptr<Expr> new_target);

  [[nodiscard]] const Expr& Value() const noexcept;
  Expr& MutableValue() noexcept;
  void SetValue(std::unique_ptr<Expr> new_value);
  std::unique_ptr<Expr> ReplaceValue(std::unique_ptr<Expr> new_value);

private:
  const IAssignOpTag* kind_ = &optags::RefAssign();
  std::unique_ptr<Expr> target_;
  std::unique_ptr<Expr> value_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ASSIGN_HPP_
