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

std::unique_ptr<Module> ParserFsm::Parse(ITokenStream& ts, IDiagnosticSink& diags) {
  ContextParser context;
  context.SetDiagnostics(&diags);
  context.SetExpr(expr_parser_.get());
  context.SetTypeParser(type_parser_.get());
  context.SetFactory(factory_.get());

  SimpleRecovery recovery;

  context.PushState(StateRegistry::Module());

  while (const IState* state = context.CurrentState()) {
    auto step = state->TryStep(context, ts);

    if (!step.has_value()) {
      const auto& message = step.error().Message();
      diags.Error("P0001", message.empty() ? "parse error" : message);
      recovery.SyncToStatementEnd(ts);
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

  if (auto* as_module = dynamic_cast<Module*>(root.get()); as_module != nullptr) {
    [[maybe_unused]] auto* realized = root.release();
    return std::unique_ptr<Module>(as_module);
  }

  return std::make_unique<Module>();
}

} // namespace ovum::compiler::parser
