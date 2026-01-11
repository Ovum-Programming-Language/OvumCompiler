#ifndef PARSER_INTLIT_HPP_
#define PARSER_INTLIT_HPP_

#include <cstdint>
#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class IntLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] int64_t Value() const noexcept;
  void SetValue(int64_t new_value) noexcept;

private:
  int64_t value_ = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_INTLIT_HPP_
