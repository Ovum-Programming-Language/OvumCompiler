#ifndef PARSER_DESTRUCTORDECL_HPP_
#define PARSER_DESTRUCTORDECL_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"

namespace ovum::compiler::parser {

class DestructorDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  bool IsPublic() const noexcept;
  void SetPublic(bool value) noexcept;

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> block);
  std::unique_ptr<Block> ReleaseBody();

private:
  bool is_public_ = true;
  std::unique_ptr<Block> body_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_DESTRUCTORDECL_HPP_
