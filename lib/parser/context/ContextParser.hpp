#ifndef PARSER_CONTEXTPARSER_HPP_
#define PARSER_CONTEXTPARSER_HPP_

#include <memory>
#include <type_traits>
#include <vector>

#include "NodeEntry.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/IState.hpp"

namespace ovum::compiler::parser {

class AstNode;
class IExpressionParser;
class ITypeParser;

template<class T>
concept AstNodeDerived = std::is_base_of_v<AstNode, T>;

class ContextParser {
public:
  std::vector<const IState*>& StateStack();
  std::vector<NodeEntry>& NodeStack();

  void SetDiagnostics(IDiagnosticSink* diagnostics);
  IDiagnosticSink* Diags() const;

  void SetExpr(IExpressionParser* parser);
  IExpressionParser* Expr() const;

  void SetTypeParser(ITypeParser* parser);
  ITypeParser* TypeParser() const;

  void PushState(const IState& state);
  void PopState();
  const IState* CurrentState() const;

  template<AstNodeDerived T>
  T* TopNodeAs() {
    if (node_stack_.empty()) {
      return nullptr;
    }

    return dynamic_cast<T*>(node_stack_.back().MutableNode());
  }

  void PushNode(std::unique_ptr<AstNode> node);
  std::unique_ptr<AstNode> PopNode();

  bool HasStates() const noexcept;
  bool HasNodes() const noexcept;
  void Clear();

private:
  std::vector<const IState*> state_stack_;
  std::vector<NodeEntry> node_stack_;
  IDiagnosticSink* diags_ = nullptr;
  IExpressionParser* expr_ = nullptr;
  ITypeParser* typep_ = nullptr;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CONTEXTPARSER_HPP_
