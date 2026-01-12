#ifndef PARSER_NAMESPACEREF_HPP_
#define PARSER_NAMESPACEREF_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class NamespaceRef : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& NamespaceExpr() const noexcept;
  [[nodiscard]] Expr& MutableNamespaceExpr() const noexcept;
  void SetNamespaceExpr(std::unique_ptr<Expr> new_namespace_expr);
  std::unique_ptr<Expr> ReplaceNamespaceExpr(std::unique_ptr<Expr> new_namespace_expr);

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

private:
  std::unique_ptr<Expr> namespace_;
  std::string name_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_NAMESPACEREF_HPP_
