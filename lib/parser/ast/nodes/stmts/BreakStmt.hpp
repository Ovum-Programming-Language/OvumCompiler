#ifndef BREAKSTMT_HPP_
#define BREAKSTMT_HPP_

#include "lib/parser/ast/nodes/base/Stmt.hpp"

class BreakStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;
};

#endif // BREAKSTMT_HPP_
