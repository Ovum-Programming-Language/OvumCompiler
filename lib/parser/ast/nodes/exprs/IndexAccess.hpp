#ifndef PARSER_INDEXACCESS_HPP_
#define PARSER_INDEXACCESS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class IndexAccess : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& Object() const noexcept;
  Expr& MutableObject() noexcept;
  void SetObject(std::unique_ptr<Expr> new_object);
  std::unique_ptr<Expr> ReplaceObject(std::unique_ptr<Expr> new_object);

  [[nodiscard]] const Expr& IndexExpr() const noexcept;
  Expr& MutableIndexExpr() noexcept;
  void SetIndexExpr(std::unique_ptr<Expr> new_index);
  std::unique_ptr<Expr> ReplaceIndexExpr(std::unique_ptr<Expr> new_index);

private:
  std::unique_ptr<Expr> object_;
  std::unique_ptr<Expr> index_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_INDEXACCESS_HPP_
