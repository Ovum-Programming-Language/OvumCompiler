#ifndef EXPRSTMT_HPP_
#define EXPRSTMT_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class ExprStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> expr_;
};

#endif // EXPRSTMT_HPP_
