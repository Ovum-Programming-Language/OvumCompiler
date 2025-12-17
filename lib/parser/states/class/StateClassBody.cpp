#include "StateClassBody.hpp"

#include <memory>

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
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

std::string_view StateClassBody::Name() const {
  return "ClassBody";
}

IState::StepResult StateClassBody::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  ClassDecl* class_decl = ctx.TopNodeAs<ClassDecl>();
  if (class_decl == nullptr) {
    return std::unexpected(StateError("expected ClassDecl node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in class body"));
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == "}") {
    ts.Consume();
    // Pop class and add to module
    auto class_node = ctx.PopNode();
    Module* module = ctx.TopNodeAs<Module>();
    if (module != nullptr) {
      module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(class_node.release())));
    }
    return false;
  }

  ctx.PushState(StateRegistry::ClassMember());
  return true;
}

}  // namespace ovum::compiler::parser
