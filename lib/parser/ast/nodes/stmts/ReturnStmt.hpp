#ifndef RETURNSTMT_HPP_
#define RETURNSTMT_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Stmt.hpp"

class Expr;
class ReturnStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> value_; // optional
};

#endif // RETURNSTMT_HPP_
