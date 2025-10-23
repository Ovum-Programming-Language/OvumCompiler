#ifndef CONTINUESTMT_HPP_
#define CONTINUESTMT_HPP_

#include "lib/parser/ast/nodes/base/Stmt.hpp"

class ContinueStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;
};

#endif // CONTINUESTMT_HPP_
