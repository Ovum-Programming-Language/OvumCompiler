#ifndef PARSER_FUNCTIONDECL_HPP_
#define PARSER_FUNCTIONDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"

namespace ovum::compiler::parser {

class TypeReference;

class FunctionDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

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
  bool is_pure_ = false;
  std::string name_;
  std::vector<Param> params_;
  std::unique_ptr<TypeReference> return_type_; // optional
  std::unique_ptr<Block> body_;                // optional
};

} // namespace ovum::compiler::parser

#endif // PARSER_FUNCTIONDECL_HPP_
