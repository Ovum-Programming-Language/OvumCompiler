#ifndef CHARLIT_HPP_
#define CHARLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class CharLit : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  char32_t value_ = U'\0';
};

#endif // CHARLIT_HPP_
