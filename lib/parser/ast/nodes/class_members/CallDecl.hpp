#ifndef PARSER_CALLDECL_HPP_
#define PARSER_CALLDECL_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class CallDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] bool IsPublic() const noexcept;
  void SetPublic(bool v) noexcept;

  [[nodiscard]] const std::vector<Param>& Params() const noexcept;
  std::vector<Param>& MutableParams() noexcept;
  void AddParam(Param param);

  [[nodiscard]] const TypeReference* ReturnType() const noexcept;
  TypeReference* MutableReturnType() noexcept;
  void SetReturnType(std::unique_ptr<TypeReference> type);
  std::unique_ptr<TypeReference> ReleaseReturnType();

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> block);
  std::unique_ptr<Block> ReleaseBody();

private:
  bool is_public_ = true;
  std::vector<Param> params_;
  std::unique_ptr<TypeReference> ret_type_;
  std::unique_ptr<Block> body_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CALLDECL_HPP_
