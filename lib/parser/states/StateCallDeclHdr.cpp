#include "StateCallDeclHdr.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/states/func/StateFuncParams.hpp"
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

} // namespace

std::string_view StateCallDeclHdr::Name() const {
  return "CallDeclHdr";
}

IState::StepResult StateCallDeclHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  auto* class_decl = ctx.TopNodeAs<ClassDecl>();
  if (class_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected ClassDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in call declaration")));
  }

  const Token& start = ts.Peek();

  // Check for access modifier
  bool is_public = true;
  if (start.GetLexeme() == "public" || start.GetLexeme() == "private") {
    is_public = (start.GetLexeme() == "public");
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "call") {
      return std::unexpected(StateError(std::string_view("expected 'call' after access modifier")));
    }
  }

  if (ts.Peek().GetLexeme() != "call") {
    return std::unexpected(StateError(std::string_view("expected 'call' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_CALL_PARAMS_OPEN", "expected '(' after 'call'");
    }
    return std::unexpected(StateError(std::string_view("expected '(' after 'call'")));
  }

  SourceSpan span = StateBase::SpanFrom(start);
  auto call = ctx.Factory()->MakeCallDecl(is_public, {}, nullptr, nullptr, span);
  ctx.PushNode(std::move(call));

  ctx.PushState(StateRegistry::FuncParams());
  return true;
}

} // namespace ovum::compiler::parser
