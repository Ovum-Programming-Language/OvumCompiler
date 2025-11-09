#ifndef FLOATLIT_HPP_
#define FLOATLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

class FloatLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  double Value() const noexcept;
  void SetValue(double new_value) noexcept;

private:
  double value = 0.0;
};

#endif // FLOATLIT_HPP_
