#ifndef IDENTREF_HPP_
#define IDENTREF_HPP_

#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class IdentRef : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::string name_;
};

#endif // IDENTREF_HPP_
