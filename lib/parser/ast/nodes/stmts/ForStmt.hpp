#ifndef FORSTMT_HPP_
#define FORSTMT_HPP_

#include <memory>
#include <string>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"

class ForStmt : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  std::string iter_name_;
  std::unique_ptr<Expr> iter_expr_;
  std::unique_ptr<Block> body_;
};

#endif // FORSTMT_HPP_
