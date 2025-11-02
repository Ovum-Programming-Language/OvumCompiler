#ifndef CONTEXTPARSER_HPP_
#define CONTEXTPARSER_HPP_

#include <memory>
#include <type_traits>
#include <vector>

#include "NodeEntry.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/IState.hpp"

class AstNode;
class IExpressionParser;
class ITypeParser;

template<class T>
concept AstNodeDerived = std::is_base_of_v<AstNode, T>;

class ContextParser {
public:
  std::vector<const IState*>& StateStack();
  std::vector<NodeEntry>& NodeStack();

  void SetDiagnostics(IDiagnosticSink* d);
  [[nodiscard]] IDiagnosticSink* Diags() const;

  void SetExpr(IExpressionParser* parser);
  [[nodiscard]] IExpressionParser* Expr() const;

  void SetTypeParser(ITypeParser* parser);
  [[nodiscard]] ITypeParser* TypeParser() const;

  void PushState(const IState& state);
  void PopState();
  [[nodiscard]] const IState* CurrentState() const;

  template<AstNodeDerived T>
  [[nodiscard]] T* TopNodeAs() {
    if (node_stack_.empty())
      return nullptr;
    return dynamic_cast<T*>(node_stack_.back().GetNode);
  }

  void PushNode(std::unique_ptr<AstNode> node);
  [[nodiscard]] std::unique_ptr<AstNode> PopNode();

private:
  std::vector<const IState*> state_stack_;
  std::vector<NodeEntry> node_stack_;
  IDiagnosticSink* diags_ = nullptr;
  IExpressionParser* expr_ = nullptr;
  ITypeParser* typep_ = nullptr;
};

#endif // CONTEXTPARSER_HPP_
