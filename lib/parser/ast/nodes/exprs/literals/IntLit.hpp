#ifndef INTLIT_HPP_
#define INTLIT_HPP_

#include <cstdint>
#include "lib/parser/ast/nodes/base/Expr.hpp"

class IntLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  int64_t Value() const noexcept;
  void SetValue(int64_t new_value) noexcept;

private:
  int64_t value = 0;
};

#endif // INTLIT_HPP_
