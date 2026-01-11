#ifndef PARSER_STATICFIELDDECL_HPP_
#define PARSER_STATICFIELDDECL_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class StaticFieldDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] bool IsPublic() const noexcept;
  void SetPublic(bool is_public) noexcept;

  [[nodiscard]] bool IsVar() const noexcept;
  void SetVar(bool is_var) noexcept;

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  [[nodiscard]] const TypeReference& Type() const noexcept;
  TypeReference& MutableType() noexcept;
  void SetType(TypeReference type);

  [[nodiscard]] const Expr* Init() const noexcept;
  Expr* MutableInit() noexcept;
  void SetInit(std::unique_ptr<Expr> expr);
  std::unique_ptr<Expr> ReleaseInit();

private:
  bool is_public_ = true;
  bool is_var_ = false;
  std::string name_;
  TypeReference type_;
  std::unique_ptr<Expr> init_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATICFIELDDECL_HPP_
