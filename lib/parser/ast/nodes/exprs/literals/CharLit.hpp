#ifndef CHARLIT_HPP_
#define CHARLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class CharLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  char Value() const noexcept;
  void SetValue(char new_value) noexcept;

private:
  char value_ = '\0';
};

#endif // CHARLIT_HPP_
