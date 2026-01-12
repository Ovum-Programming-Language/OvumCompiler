#include "StateInterfaceBody.hpp"

#include <memory>

#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT") {
      ts.Consume();
      continue;
    }
    if (type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

} // namespace

std::string_view StateInterfaceBody::Name() const {
  return "InterfaceBody";
}

IState::StepResult StateInterfaceBody::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (const auto* interface_decl = ctx.TopNodeAs<InterfaceDecl>(); interface_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected InterfaceDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in interface body")));
  }

  if (const Token& tok = ts.Peek(); tok.GetLexeme() == "}") {
    ts.Consume();
    // Pop interface and add to module
    auto interface_node = ctx.PopNode();
    if (auto* module = ctx.TopNodeAs<Module>(); module != nullptr) {
      module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(interface_node.release())));
    }
    return false;
  }

  ctx.PushState(StateRegistry::InterfaceDecl());
  return true;
}

} // namespace ovum::compiler::parser
