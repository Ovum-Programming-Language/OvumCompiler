#ifndef UNSAFEBLOCK_HPP_
#define UNSAFEBLOCK_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class UnsafeBlock : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  const Block* Body() const noexcept;
  Block* MutableBody() noexcept;
  void SetBody(std::unique_ptr<Block> body_block);
  std::unique_ptr<Block> ReleaseBody();

private:
  std::unique_ptr<Block> body_;
};

#endif // UNSAFEBLOCK_HPP_
