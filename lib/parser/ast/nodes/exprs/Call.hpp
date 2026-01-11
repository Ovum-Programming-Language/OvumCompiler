#ifndef PARSER_CALL_HPP_
#define PARSER_CALL_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class Call : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& Callee() const noexcept;
  Expr& MutableCallee() noexcept;
  void SetCallee(std::unique_ptr<Expr> new_callee);
  std::unique_ptr<Expr> ReplaceCallee(std::unique_ptr<Expr> new_callee);

  [[nodiscard]] const std::vector<std::unique_ptr<Expr>>& Args() const noexcept;
  std::vector<std::unique_ptr<Expr>>& MutableArgs() noexcept;
  void AddArg(std::unique_ptr<Expr> new_arg);
  void ClearArgs();

private:
  std::unique_ptr<Expr> callee_;
  std::vector<std::unique_ptr<Expr>> args_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CALL_HPP_
