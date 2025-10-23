#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/AstNode.hpp"

class NodeEntry {
public:
  std::unique_ptr<NodeEntry> GetNode;

private:
  std::unique_ptr<AstNode> node_;
};

#endif // NODEENTRY_HPP_
