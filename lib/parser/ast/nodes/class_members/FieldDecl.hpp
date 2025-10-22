#ifndef FIELDDECL_HPP_
#define FIELDDECL_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

class FieldDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_public_ = true;
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<class Expr> init_; // optional
};

#endif // FIELDDECL_HPP_
