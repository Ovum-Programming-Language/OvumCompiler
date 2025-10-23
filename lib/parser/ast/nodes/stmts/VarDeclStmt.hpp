#ifndef VARDECLSTMT_HPP_
#define VARDECLSTMT_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"
#include "lib/parser/types/TypeReference.hpp"

class VarDeclStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_;
};

#endif // VARDECLSTMT_HPP_
