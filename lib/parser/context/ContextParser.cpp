#include "ContextParser.hpp"

#include <utility>

namespace ovum::compiler::parser {

std::vector<const IState*>& ContextParser::StateStack() {
  return state_stack_;
}

std::vector<NodeEntry>& ContextParser::NodeStack() {
  return node_stack_;
}

void ContextParser::SetDiagnostics(IDiagnosticSink* diagnostics) {
  diags_ = diagnostics;
}

IDiagnosticSink* ContextParser::Diags() const {
  return diags_;
}

void ContextParser::SetExpr(IExpressionParser* parser) {
  expr_ = parser;
}

IExpressionParser* ContextParser::Expr() const {
  return expr_;
}

void ContextParser::SetTypeParser(ITypeParser* parser) {
  typep_ = parser;
}

ITypeParser* ContextParser::TypeParser() const {
  return typep_;
}

void ContextParser::PushState(const IState& state) {
  state_stack_.push_back(&state);
}

void ContextParser::PopState() {
  if (!state_stack_.empty()) {
    state_stack_.pop_back();
  }
}

const IState* ContextParser::CurrentState() const {
  if (state_stack_.empty()) {
    return nullptr;
  }

  return state_stack_.back();
}

void ContextParser::PushNode(std::unique_ptr<AstNode> node) {
  node_stack_.emplace_back(std::move(node));
}

std::unique_ptr<AstNode> ContextParser::PopNode() {
  if (node_stack_.empty()) {
    return nullptr;
  }

  auto out = node_stack_.back().ReleaseNode();
  node_stack_.pop_back();
  return out;
}

bool ContextParser::HasStates() const noexcept {
  return !state_stack_.empty();
}

bool ContextParser::HasNodes() const noexcept {
  return !node_stack_.empty();
}

void ContextParser::Clear() {
  state_stack_.clear();
  node_stack_.clear();
  diags_ = nullptr;
  expr_ = nullptr;
  typep_ = nullptr;
}

} // namespace ovum::compiler::parser
