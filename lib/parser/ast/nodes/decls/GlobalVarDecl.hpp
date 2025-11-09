#ifndef PARSER_GLOBALVARDECL_HPP_
#define PARSER_GLOBALVARDECL_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class GlobalVarDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  bool IsVar() const noexcept;
  void SetVar(bool is_var) noexcept;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference type);

  const Expr* Init() const noexcept;
  Expr* MutableInit() noexcept;
  void SetInit(std::unique_ptr<Expr> expr);
  std::unique_ptr<Expr> ReleaseInit();

private:
  bool is_var_ = false; // var=true, val=false
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_; // optional
};

} // namespace ovum::compiler::parser

#endif // PARSER_GLOBALVARDECL_HPP_
