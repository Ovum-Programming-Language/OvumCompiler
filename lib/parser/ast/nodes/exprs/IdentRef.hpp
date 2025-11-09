#ifndef IDENTREF_HPP_
#define IDENTREF_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class IdentRef : public Expr {
public:
  void Accept(AstVisitor& visitor) override;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

private:
  std::string name_;
};

#endif // IDENTREF_HPP_
