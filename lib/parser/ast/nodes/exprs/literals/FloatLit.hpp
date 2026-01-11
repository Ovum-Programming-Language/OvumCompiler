#ifndef PARSER_FLOATLIT_HPP_
#define PARSER_FLOATLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class FloatLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] double Value() const noexcept;
  void SetValue(double new_value) noexcept;

private:
  double value_ = 0.0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_FLOATLIT_HPP_
