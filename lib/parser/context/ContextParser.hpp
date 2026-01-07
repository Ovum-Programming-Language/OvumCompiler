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
class IAstFactory; // forward

template<class T>
concept AstNodeDerived = std::is_base_of_v<AstNode, T>;

class ContextParser {
public:
  std::vector<const IState*>& StateStack();
  std::vector<NodeEntry>& NodeStack();

  void SetDiagnostics(IDiagnosticSink* diagnostics);
  [[nodiscard]] IDiagnosticSink* Diags() const;

  void SetExpr(IExpressionParser* parser);
  [[nodiscard]] IExpressionParser* Expr() const;

  void SetTypeParser(ITypeParser* parser);
  [[nodiscard]] ITypeParser* TypeParser() const;

  void SetFactory(IAstFactory* factory);
  [[nodiscard]] IAstFactory* Factory() const;

  void PushState(const IState& state);
  void PopState();
  [[nodiscard]] const IState* CurrentState() const;

  template<AstNodeDerived T>
  T* TopNodeAs() {
    if (node_stack_.empty()) {
      return nullptr;
    }

    AstNode* base = node_stack_.back().MutableNode();
    if (!base) {
      return nullptr;
    }

    return dynamic_cast<T*>(base);
  }

  void PushNode(std::unique_ptr<AstNode> node);
  std::unique_ptr<AstNode> PopNode();

  [[nodiscard]] bool HasStates() const noexcept;
  [[nodiscard]] bool HasNodes() const noexcept;
  void Clear();

private:
  std::vector<const IState*> state_stack_;
  std::vector<NodeEntry> node_stack_;
  IDiagnosticSink* diags_ = nullptr;
  IExpressionParser* expr_ = nullptr;
  ITypeParser* typep_ = nullptr;
  IAstFactory* factory_ = nullptr;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CONTEXTPARSER_HPP_
