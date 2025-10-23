#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

class Block : public Stmt {
public:
  void Accept(AstVisitor& v) override;

  std::vector<std::unique_ptr<Stmt>>& GetStatements();
  const std::vector<std::unique_ptr<Stmt>>& GetStatements() const;
  void Append(std::unique_ptr<Stmt> s);

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;
};

#endif // BLOCK_HPP_
