#ifndef PARSER_NODEENTRY_HPP_
#define PARSER_NODEENTRY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/AstNode.hpp"

namespace ovum::compiler::parser {

class NodeEntry {
public:
  NodeEntry() = default;
  explicit NodeEntry(std::unique_ptr<AstNode> node);

  [[nodiscard]] const AstNode* GetNode() const noexcept;
  [[nodiscard]] AstNode* MutableNode() const noexcept;
  void SetNode(std::unique_ptr<AstNode> node);
  std::unique_ptr<AstNode> ReleaseNode();

private:
  std::unique_ptr<AstNode> node_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_NODEENTRY_HPP_
