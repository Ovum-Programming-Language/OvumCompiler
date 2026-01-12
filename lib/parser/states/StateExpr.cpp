#include "StateExpr.hpp"

#include <memory>

#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "pratt/IExpressionParser.hpp"

namespace ovum::compiler::parser {

std::string_view StateExpr::Name() const {
  return "Expr";
}

IState::StepResult StateExpr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  if (ctx.Expr() == nullptr) {
    return std::unexpected(StateError(std::string_view("expression parser not available")));
  }

  auto expr = ctx.Expr()->Parse(ts, *ctx.Diags());
  if (expr == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse expression")));
  }

  ctx.PushNode(std::move(expr));
  return false;
}

} // namespace ovum::compiler::parser
