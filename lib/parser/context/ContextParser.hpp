#ifndef CONTEXTPARSER_HPP_
#define CONTEXTPARSER_HPP_

#include <vector>

#include "NodeEntry.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/IState.hpp"

class IExpressionParser; // forward
class ITypeParser;       // forward

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
  const IState& CurrentState() const;

  template<class T>
  T* TopNodeAs() {
    if (node_stack_.empty()) {
      return nullptr;
    }

    return dynamic_cast<T*>(node_stack_.back());
  }

  void PushNode(std::unique_ptr<AstNode> node);
  std::unique_ptr<AstNode> PopNode();

private:
  std::vector<const IState*> state_stack_;
  std::vector<NodeEntry> node_stack_;
  IDiagnosticSink* diags_ = nullptr;
  IExpressionParser* expr_ = nullptr;
  ITypeParser* typep_ = nullptr;
};

#endif // CONTEXTPARSER_HPP_
