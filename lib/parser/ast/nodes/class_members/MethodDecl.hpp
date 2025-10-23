#ifndef METHODDECL_HPP_
#define METHODDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

class MethodDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_public_ = true;
  bool is_override_ = false;
  bool is_static_ = false;
  bool is_pure_ = false;

  std::string name;
  std::vector<Param> params;
  std::unique_ptr<TypeReference> ret_type;
  std::unique_ptr<Block> body; // optional
};

#endif // METHODDECL_HPP_
