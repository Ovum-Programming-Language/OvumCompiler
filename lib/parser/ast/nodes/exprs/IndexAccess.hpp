#ifndef INDEXACCESS_HPP_
#define INDEXACCESS_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"

class IndexAccess : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Expr> object_;
  std::unique_ptr<Expr> index_;
};

#endif // INDEXACCESS_HPP_
