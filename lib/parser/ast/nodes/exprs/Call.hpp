#ifndef CALL_HPP_
#define CALL_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class Call : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> callee_;
  std::vector<std::unique_ptr<Expr>> args_;
};

#endif // CALL_HPP_
