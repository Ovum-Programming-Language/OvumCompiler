#ifndef PARSER_BOOLLIT_HPP_
#define PARSER_BOOLLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class BoolLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] bool Value() const noexcept;
  void SetValue(bool new_value) noexcept;

private:
  bool value_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BOOLLIT_HPP_
