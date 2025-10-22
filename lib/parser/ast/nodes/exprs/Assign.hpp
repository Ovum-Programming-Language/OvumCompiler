#ifndef ASSIGN_HPP_
#define ASSIGN_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "tags/IAssignOpTag.hpp"
#include "tags/OpTags.hpp"

class Assign : public Expr {
public:
  void Accept(AstVisitor& v) override;

private:
  const IAssignOpTag* kind_ = &OpTags::RefAssign();
  std::unique_ptr<Expr> target_;
  std::unique_ptr<Expr> value_;
};

#endif // ASSIGN_HPP_
