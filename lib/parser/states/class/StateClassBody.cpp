#include "StateClassBody.hpp"

#include <memory>

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/context/ContextParser.hpp"
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

std::string_view StateClassBody::Name() const {
  return "ClassBody";
}

IState::StepResult StateClassBody::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (const auto* class_decl = ctx.TopNodeAs<ClassDecl>(); class_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected ClassDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in class body")));
  }

  if (const Token& tok = ts.Peek(); tok.GetLexeme() == "}") {
    ts.Consume();
    // Pop class and add to module
    auto class_node = ctx.PopNode();
    if (auto* module = ctx.TopNodeAs<Module>(); module != nullptr) {
      module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(class_node.release())));
    }
    return false;
  }

  ctx.PushState(StateRegistry::ClassMember());
  return true;
}

} // namespace ovum::compiler::parser
