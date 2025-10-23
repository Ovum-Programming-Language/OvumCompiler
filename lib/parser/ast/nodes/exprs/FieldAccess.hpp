#ifndef FIELDACCESS_HPP_
#define FIELDACCESS_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class FieldAccess : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> object_;
  std::string name_;
};

#endif // FIELDACCESS_HPP_
