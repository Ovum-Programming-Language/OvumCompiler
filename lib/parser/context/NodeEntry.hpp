#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/AstNode.hpp"

class NodeEntry {
public:
  NodeEntry() = default;
  explicit NodeEntry(std::unique_ptr<AstNode> node);

  const AstNode* GetNode() const noexcept;
  AstNode* MutableNode() noexcept;
  void SetNode(std::unique_ptr<AstNode> node);
  std::unique_ptr<AstNode> ReleaseNode();

private:
  std::unique_ptr<AstNode> node_;
};

#endif // NODEENTRY_HPP_
