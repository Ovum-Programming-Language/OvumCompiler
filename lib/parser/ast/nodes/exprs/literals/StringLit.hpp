#ifndef PARSER_STRINGLIT_HPP_
#define PARSER_STRINGLIT_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class StringLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const std::string& Value() const noexcept;
  void SetValue(std::string new_value);

private:
  std::string value_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STRINGLIT_HPP_
