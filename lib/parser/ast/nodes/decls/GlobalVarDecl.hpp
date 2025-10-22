#ifndef GLOBALVARDECL_HPP_
#define GLOBALVARDECL_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class GlobalVarDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_var_ = false; // var=true, val=false
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_; // optional
};

#endif // GLOBALVARDECL_HPP_
