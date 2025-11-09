#ifndef PARSER_CHARLIT_HPP_
#define PARSER_CHARLIT_HPP_

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class CharLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  char Value() const noexcept;
  void SetValue(char new_value) noexcept;

private:
  char value_ = '\0';
};

} // namespace ovum::compiler::parser

#endif // PARSER_CHARLIT_HPP_
