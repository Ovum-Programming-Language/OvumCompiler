#ifndef VARDECLSTMT_HPP_
#define VARDECLSTMT_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"
#include "lib/parser/types/TypeReference.hpp"

class VarDeclStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  bool IsVar() const noexcept;
  void SetVar(bool is_var) noexcept;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference new_type);

  const Expr* Init() const noexcept;
  Expr* MutableInit() noexcept;
  void SetInit(std::unique_ptr<Expr> init_expr);
  std::unique_ptr<Expr> ReleaseInit();

private:
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_;
};

#endif // VARDECLSTMT_HPP_
