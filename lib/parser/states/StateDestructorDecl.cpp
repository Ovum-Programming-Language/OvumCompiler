#include "StateDestructorDecl.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
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

std::string_view StateDestructorDecl::Name() const {
  return "DestructorDecl";
}

IState::StepResult StateDestructorDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (auto* class_decl = ctx.TopNodeAs<ClassDecl>(); class_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected ClassDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in destructor")));
  }

  const Token& start = ts.Peek();

  bool is_public = true;
  if (start.GetLexeme() == "public" || start.GetLexeme() == "private") {
    is_public = start.GetLexeme() == "public";
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "destructor") {
      return std::unexpected(StateError(std::string_view("expected 'destructor' after access modifier")));
    }
  }

  if (ts.Peek().GetLexeme() != "destructor") {
    return std::unexpected(StateError(std::string_view("expected 'destructor' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags()) {
      ctx.Diags()->Error("P_DESTRUCTOR_PARAMS", "expected '(' after 'destructor'");
    }
    return std::unexpected(StateError(std::string_view("expected '(' after 'destructor'")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ")") {
    if (ctx.Diags()) {
      ctx.Diags()->Error("P_DESTRUCTOR_PARAMS_CLOSE", "expected ')' in destructor");
    }
    return std::unexpected(StateError(std::string_view("expected ')' in destructor")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
    if (ctx.Diags()) {
      ctx.Diags()->Error("P_DESTRUCTOR_RETURN", "expected ':' after parameters");
    }
    return std::unexpected(StateError(std::string_view("expected ':' after parameters")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "Void") {
    if (ctx.Diags()) {
      ctx.Diags()->Error("P_DESTRUCTOR_VOID", "expected 'Void' return type for destructor");
    }
    return std::unexpected(StateError(std::string_view("expected 'Void' return type")));
  }
  ts.Consume();

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags()) {
      ctx.Diags()->Error("P_DESTRUCTOR_BODY", "expected '{' for destructor body");
    }
    return std::unexpected(StateError(std::string_view("expected '{' for destructor body")));
  }

  ts.Consume();

  auto destructor = ctx.Factory()->MakeDestructor(is_public, nullptr, SpanFrom(start));
  ctx.PushNode(std::move(destructor));

  auto body = ctx.Factory()->MakeBlock({}, SpanFrom(start));
  ctx.PushNode(std::move(body));

  ctx.PushState(StateRegistry::Block());

  return true;
}

} // namespace ovum::compiler::parser
