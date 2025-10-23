#ifndef STRINGLIT_HPP_
#define STRINGLIT_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class StringLit : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::u32string value_;
};

#endif // STRINGLIT_HPP_
