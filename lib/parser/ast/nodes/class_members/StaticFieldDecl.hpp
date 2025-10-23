#ifndef STATICFIELDDECL_HPP_
#define STATICFIELDDECL_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class StaticFieldDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_public_ = true;
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_;
};

#endif // STATICFIELDDECL_HPP_
