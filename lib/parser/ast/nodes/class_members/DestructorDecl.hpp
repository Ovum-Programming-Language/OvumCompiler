#ifndef DESTRUCTORDECL_HPP_
#define DESTRUCTORDECL_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"

class DestructorDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_public_ = true;
  std::unique_ptr<Block> body_;
};

#endif // DESTRUCTORDECL_HPP_
