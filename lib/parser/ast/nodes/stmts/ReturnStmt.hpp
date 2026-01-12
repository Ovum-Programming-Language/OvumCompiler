#ifndef PARSER_RETURNSTMT_HPP_
#define PARSER_RETURNSTMT_HPP_

#include <memory>
#include <optional>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class ReturnStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] bool HasValue() const noexcept;
  [[nodiscard]] const Expr* Value() const noexcept;
  Expr* MutableValue() noexcept;
  void SetValue(std::unique_ptr<Expr> new_value);
  void ResetValue();
  std::unique_ptr<Expr> ReleaseValue();

private:
  std::optional<std::unique_ptr<Expr>> value_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_RETURNSTMT_HPP_
