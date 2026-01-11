#ifndef PARSER_IDENTREF_HPP_
#define PARSER_IDENTREF_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class IdentRef : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

private:
  std::string name_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IDENTREF_HPP_
