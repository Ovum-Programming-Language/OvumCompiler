#include "StateFuncBody.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"

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

SourceSpan Union(const SourceSpan& a, const SourceSpan& b) {
  return StateBase::Union(a, b);
}

} // namespace

std::string_view StateFuncBody::Name() const {
  return "FuncBody";
}

IState::StepResult StateFuncBody::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  // Check for FunctionDecl, MethodDecl, or CallDecl
  FunctionDecl* func = ctx.TopNodeAs<FunctionDecl>();
  MethodDecl* method = ctx.TopNodeAs<MethodDecl>();
  CallDecl* call = ctx.TopNodeAs<CallDecl>();

  if (func == nullptr && method == nullptr && call == nullptr) {
    return std::unexpected(
        StateError(std::string_view("expected FunctionDecl, MethodDecl, or CallDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in function body")));
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == ";") {
    // Declaration without body
    ts.Consume();
    auto decl_node = ctx.PopNode();

    if (func != nullptr) {
      Module* module = ctx.TopNodeAs<Module>();
      if (module != nullptr) {
        module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(decl_node.release())));
      }
    } else if (method != nullptr || call != nullptr) {
      ClassDecl* class_decl = ctx.TopNodeAs<ClassDecl>();
      if (class_decl != nullptr) {
        class_decl->AddMember(std::unique_ptr<Decl>(dynamic_cast<Decl*>(decl_node.release())));
      }
    }
    return false;
  }

  if (tok.GetLexeme() == "{") {
    // Function with body - create block and push it
    ts.Consume();
    auto block = ctx.Factory()->MakeBlock({}, StateBase::SpanFrom(tok));
    ctx.PushNode(std::unique_ptr<AstNode>(block.get()));
    ctx.PushState(StateRegistry::Block());
    return true;
  }

  if (ctx.Diags() != nullptr) {
    ctx.Diags()->Error("P_FUN_BODY", std::string_view("expected '{' or ';' for function body"));
  }
  return std::unexpected(StateError(std::string_view("expected '{' or ';' for function body")));
}

} // namespace ovum::compiler::parser
