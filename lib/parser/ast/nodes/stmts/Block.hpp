#ifndef PARSER_BLOCK_HPP_
#define PARSER_BLOCK_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class Block : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;

  std::vector<std::unique_ptr<Stmt>>& GetStatements();
  const std::vector<std::unique_ptr<Stmt>>& GetStatements() const;
  void Append(std::unique_ptr<Stmt> statement);

  void Insert(std::size_t index, std::unique_ptr<Stmt> statement);
  std::unique_ptr<Stmt> ReleaseAt(std::size_t index);
  void Clear() noexcept;
  std::size_t Size() const noexcept;

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BLOCK_HPP_
