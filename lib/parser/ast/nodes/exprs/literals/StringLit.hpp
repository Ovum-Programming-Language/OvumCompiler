#ifndef STRINGLIT_HPP_
#define STRINGLIT_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class StringLit : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const std::string& Value() const noexcept;
  void SetValue(std::string new_value);

private:
  std::string value_;
};

#endif // STRINGLIT_HPP_
