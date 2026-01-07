#include "ParserFsm.hpp"

#include <memory>
#include <utility>

#include "context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/IState.hpp"
#include "lib/parser/states/base/StateError.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "recovery/SimpleRecovery.hpp"

namespace ovum::compiler::parser {

ParserFsm::ParserFsm(std::unique_ptr<IExpressionParser> expr,
                     std::unique_ptr<ITypeParser> typep,
                     std::shared_ptr<IAstFactory> factory) :
    expr_parser_(std::move(expr)), type_parser_(std::move(typep)), factory_(std::move(factory)) {
}

std::unique_ptr<Module> ParserFsm::Parse(ITokenStream& token_stream, IDiagnosticSink& diagnostics) {
  ContextParser context;
  context.SetDiagnostics(&diagnostics);
  context.SetExpr(expr_parser_.get());
  context.SetTypeParser(type_parser_.get());
  context.SetFactory(factory_.get());

  SimpleRecovery recovery;

  context.PushState(StateRegistry::Module());

  while (const IState* state = context.CurrentState()) {
    auto name = state->Name();
    auto step = state->TryStep(context, token_stream);

    if (!step.has_value()) {
      const auto& message = step.error().Message();
      diagnostics.Error("P0001", message.empty() ? "parse error" : message);
      recovery.SyncToStatementEnd(token_stream);
      context.PopState();
      continue;
    }

    if (!*step) {
      context.PopState();
    }
  }

  std::unique_ptr<AstNode> root = context.PopNode();

  if (!root) {
    return std::make_unique<Module>();
  }

  auto* as_module = dynamic_cast<Module*>(root.get());

  if (as_module != nullptr) {
    root.release();
    return std::unique_ptr<Module>(as_module);
  }

  return std::make_unique<Module>();
}

} // namespace ovum::compiler::parser
