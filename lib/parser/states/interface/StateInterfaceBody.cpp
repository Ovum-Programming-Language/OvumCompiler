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

void SkipTrivia(ITokenStream& ts, bool skip_newlines = true) {
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "COMMENT") {
      ts.Consume();
      continue;
    }
    if (skip_newlines && type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    break;
  }
}

}  // namespace

std::string_view StateInterfaceBody::Name() const {
  return "InterfaceBody";
}

IState::StepResult StateInterfaceBody::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  InterfaceDecl* interface_decl = ctx.TopNodeAs<InterfaceDecl>();
  if (interface_decl == nullptr) {
    return std::unexpected(StateError("expected InterfaceDecl node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in interface body"));
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == "}") {
    ts.Consume();
    // Pop interface and add to module
    auto interface_node = ctx.PopNode();
    Module* module = ctx.TopNodeAs<Module>();
    if (module != nullptr) {
      module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(interface_node.release())));
    }
    return false;
  }

  ctx.PushState(StateRegistry::InterfaceDecl());
  return true;
}

}  // namespace ovum::compiler::parser
