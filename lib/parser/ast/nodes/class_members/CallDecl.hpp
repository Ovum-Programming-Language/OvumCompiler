#ifndef CALLDECL_HPP_
#define CALLDECL_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

class CallDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  bool is_public_ = true;
  std::vector<Param> params_;
  std::unique_ptr<TypeReference> ret_type_;
  std::unique_ptr<Block> body_;
};

#endif // CALLDECL_HPP_
