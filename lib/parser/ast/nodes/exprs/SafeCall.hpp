#ifndef SAFECALL_HPP_
#define SAFECALL_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class SafeCall : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> base; // TODO: idk how to implement it now...
};

#endif // SAFECALL_HPP_
