#ifndef FUNCTIONDECL_HPP_
#define FUNCTIONDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"

class FunctionDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_pure_ = false;
  std::string name_;
  std::vector<Param> params_;
  std::unique_ptr<TypeReference> return_type_; // optional
  std::unique_ptr<Block> body_;                // optional
};

#endif // FUNCTIONDECL_HPP_
