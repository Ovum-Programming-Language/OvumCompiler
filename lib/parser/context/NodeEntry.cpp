#include "NodeEntry.hpp"

#include <utility>

namespace ovum::compiler::parser {

NodeEntry::NodeEntry(std::unique_ptr<AstNode> node) : node_(std::move(node)) {
}

const AstNode* NodeEntry::GetNode() const noexcept {
  return node_.get();
}

AstNode* NodeEntry::MutableNode() const noexcept {
  return node_.get();
}

void NodeEntry::SetNode(std::unique_ptr<AstNode> node) {
  node_ = std::move(node);
}

std::unique_ptr<AstNode> NodeEntry::ReleaseNode() {
  return std::move(node_);
}

} // namespace ovum::compiler::parser
