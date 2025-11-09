#ifndef BOOLLIT_HPP_
#define BOOLLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class BoolLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  bool Value() const noexcept;
  void SetValue(bool new_value) noexcept;

private:
  bool value = false;
};

#endif // BOOLLIT_HPP_
