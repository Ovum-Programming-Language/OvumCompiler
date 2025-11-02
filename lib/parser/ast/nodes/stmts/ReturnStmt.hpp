#ifndef RETURNSTMT_HPP_
#define RETURNSTMT_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class ReturnStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

  bool HasValue() const noexcept;
  const Expr* Value() const noexcept;
  Expr* MutableValue() noexcept;
  void SetValue(std::unique_ptr<Expr> e);
  void ResetValue();
  std::unique_ptr<Expr> ReleaseValue();

private:
  std::optional<std::unique_ptr<Expr>> value_;
};

#endif // RETURNSTMT_HPP_
