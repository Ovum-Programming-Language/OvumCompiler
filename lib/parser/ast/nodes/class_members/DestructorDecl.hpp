#ifndef DESTRUCTORDECL_HPP_
#define DESTRUCTORDECL_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"

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

#endif // DESTRUCTORDECL_HPP_
