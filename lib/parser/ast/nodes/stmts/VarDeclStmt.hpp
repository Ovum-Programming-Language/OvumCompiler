#ifndef PARSER_VARDECLSTMT_HPP_
#define PARSER_VARDECLSTMT_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class VarDeclStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] bool IsVar() const noexcept;
  void SetVar(bool is_var) noexcept;

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  [[nodiscard]] const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference new_type);

  [[nodiscard]] const Expr* Init() const noexcept;
  Expr* MutableInit() noexcept;
  void SetInit(std::unique_ptr<Expr> init_expr);
  std::unique_ptr<Expr> ReleaseInit();

private:
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_VARDECLSTMT_HPP_
