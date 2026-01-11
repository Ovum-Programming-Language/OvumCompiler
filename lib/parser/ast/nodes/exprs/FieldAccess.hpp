#ifndef PARSER_FIELDACCESS_HPP_
#define PARSER_FIELDACCESS_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

namespace ovum::compiler::parser {

class FieldAccess : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const Expr& Object() const noexcept;
  Expr& MutableObject() noexcept;
  void SetObject(std::unique_ptr<Expr> new_object);
  std::unique_ptr<Expr> ReplaceObject(std::unique_ptr<Expr> new_object);

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

private:
  std::unique_ptr<Expr> object_;
  std::string name_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_FIELDACCESS_HPP_
