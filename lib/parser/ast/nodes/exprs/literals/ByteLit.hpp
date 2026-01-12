#ifndef PARSER_BYTELIT_HPP_
#define PARSER_BYTELIT_HPP_

#include <cstdint>
#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class ByteLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] uint8_t Value() const noexcept;
  void SetValue(uint8_t new_value) noexcept;

private:
  uint8_t value = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BYTELIT_HPP_
