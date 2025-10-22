#ifndef NAMESPACEREF_HPP_
#define NAMESPACEREF_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class NamespaceRef : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> namespace_;
  std::string name_;
};

#endif // NAMESPACEREF_HPP_
