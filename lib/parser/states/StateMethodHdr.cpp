#include "StateMethodHdr.hpp"

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/states/func/StateFuncParams.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"

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

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(ContextParser& ctx, ITokenStream& ts,
                           std::string_view code, std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      const Token* tok = ts.TryPeek();
      if (tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error(code, message, span);
      } else {
        ctx.Diags()->Error(code, message);
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

}  // namespace

std::string_view StateMethodHdr::Name() const {
  return "MethodHdr";
}

IState::StepResult StateMethodHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  ClassDecl* class_decl = ctx.TopNodeAs<ClassDecl>();
  if (class_decl == nullptr) {
    return std::unexpected(StateError("expected ClassDecl node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in method header"));
  }

  const Token& start = ts.Peek();
  
  // Check for access modifier, override, pure, static
  bool is_public = true;
  bool is_override = false;
  bool is_pure = false;
  bool is_static = false;
  
  std::string lex = start.GetLexeme();
  if (lex == "public" || lex == "private") {
    is_public = (lex == "public");
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      return std::unexpected(StateError("unexpected end of file after access modifier"));
    }
    lex = ts.Peek().GetLexeme();
  }
  
  if (lex == "override") {
    is_override = true;
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      return std::unexpected(StateError("unexpected end of file after 'override'"));
    }
    lex = ts.Peek().GetLexeme();
  }
  
  if (lex == "pure") {
    is_pure = true;
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "fun") {
      return std::unexpected(StateError("expected 'fun' after 'pure'"));
    }
    lex = "fun";
  }
  
  if (lex == "static") {
    is_static = true;
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "fun") {
      return std::unexpected(StateError("expected 'fun' after 'static'"));
    }
    lex = "fun";
  }

  if (lex != "fun") {
    return std::unexpected(StateError("expected 'fun' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts, "P_METHOD_NAME", "expected method name");
  if (name.empty()) {
    return std::unexpected(StateError("expected method name"));
  }

  SourceSpan span = StateBase::SpanFrom(start);
  auto method = ctx.Factory()->MakeMethod(is_public, is_override, is_static, is_pure,
                                          std::move(name), {}, nullptr, nullptr, span);
  ctx.PushNode(std::unique_ptr<AstNode>(method.get()));

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_METHOD_PARAMS_OPEN", "expected '(' after method name");
    }
    return std::unexpected(StateError("expected '(' after method name"));
  }

  ctx.PushState(StateRegistry::FuncParams());
  return true;
}

}  // namespace ovum::compiler::parser
