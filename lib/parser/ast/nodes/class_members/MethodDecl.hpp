#ifndef PARSER_METHODDECL_HPP_
#define PARSER_METHODDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class MethodDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  bool IsPublic() const noexcept;
  void SetPublic(bool is_public) noexcept;

  bool IsOverride() const noexcept;
  void SetOverride(bool is_override) noexcept;

  bool IsStatic() const noexcept;
  void SetStatic(bool is_static) noexcept;

  bool IsPure() const noexcept;
  void SetPure(bool is_pure) noexcept;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const std::vector<Param>& Params() const noexcept;
  std::vector<Param>& MutableParams() noexcept;
  void AddParam(Param param);

  const TypeReference* ReturnType() const noexcept;
  TypeReference* MutableReturnType() noexcept;
  void SetReturnType(std::unique_ptr<TypeReference> type);
  std::unique_ptr<TypeReference> ReleaseReturnType();

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> block);
  std::unique_ptr<Block> ReleaseBody();

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

} // namespace ovum::compiler::parser

#endif // PARSER_METHODDECL_HPP_
