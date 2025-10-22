#ifndef UNSAFEBLOCK_HPP_
#define UNSAFEBLOCK_HPP_

#include <memory>

#include "Block.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class UnsafeBlock : public Stmt {
public:
  void Accept(AstVisitor& v) override;

private:
  std::unique_ptr<Block> body_;
};

#endif // UNSAFEBLOCK_HPP_
