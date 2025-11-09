#ifndef CALL_HPP_
#define CALL_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class Call : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const Expr& Callee() const noexcept;
  Expr& MutableCallee() noexcept;
  void SetCallee(std::unique_ptr<Expr> new_callee);
  std::unique_ptr<Expr> ReplaceCallee(std::unique_ptr<Expr> new_callee);

  const std::vector<std::unique_ptr<Expr>>& Args() const noexcept;
  std::vector<std::unique_ptr<Expr>>& MutableArgs() noexcept;
  void AddArg(std::unique_ptr<Expr> new_arg);
  void ClearArgs();

private:
  std::unique_ptr<Expr> callee_;
  std::vector<std::unique_ptr<Expr>> args_;
};

#endif // CALL_HPP_
