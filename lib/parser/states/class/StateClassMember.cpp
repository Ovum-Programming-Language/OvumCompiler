#include "StateClassMember.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "pratt/IExpressionParser.hpp"
#include "type_parser/ITypeParser.hpp"

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

std::string ReadIdentifier(const ContextParser& ctx, ITokenStream& ts, std::string_view code, std::string_view message) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
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

void ConsumeTerminators(ITokenStream& ts) {
  SkipTrivia(ts, false);
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    if (const std::string type = t.GetStringType(); type == "NEWLINE") {
      ts.Consume();
      continue;
    }
    if (t.GetLexeme() == ";") {
      ts.Consume();
      continue;
    }
    break;
  }
}

} // namespace

std::string_view StateClassMember::Name() const {
  return "ClassMember";
}

IState::StepResult StateClassMember::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  auto* class_decl = ctx.TopNodeAs<ClassDecl>();
  if (class_decl == nullptr) {
    return std::unexpected(StateError(std::string_view("expected ClassDecl node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in class member")));
  }

  const Token& start = ts.Peek();
  const size_t start_index = ts.Position();
  std::string lex = start.GetLexeme();
  SourceSpan span = SpanFrom(start);

  // Check for access modifier
  bool is_public = true;
  if (lex == "public" || lex == "private") {
    is_public = lex == "public";
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      return std::unexpected(StateError(std::string_view("unexpected end of file after access modifier")));
    }
    lex = ts.Peek().GetLexeme();
  }

  // Check for static field
  if (lex == "static") {
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      return std::unexpected(StateError(std::string_view("unexpected end of file after 'static'")));
    }

    // Access modifier after static
    if (ts.Peek().GetLexeme() == "public" || ts.Peek().GetLexeme() == "private") {
      is_public = ts.Peek().GetLexeme() == "public";
      ts.Consume();
      SkipTrivia(ts);
    }

    if (ts.IsEof()) {
      return std::unexpected(StateError(std::string_view("unexpected end of file in static field")));
    }

    bool is_var = false;
    if (ts.Peek().GetLexeme() == "var" || ts.Peek().GetLexeme() == "val") {
      is_var = ts.Peek().GetLexeme() == "var";
      ts.Consume();
      SkipTrivia(ts);
    }

    std::string name = ReadIdentifier(ctx, ts, "P_STATIC_FIELD_NAME", "expected static field name");
    if (name.empty()) {
      return std::unexpected(StateError(std::string_view("expected static field name")));
    }

    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_STATIC_FIELD_COLON", std::string_view("expected ':' after static field name"));
      }
      return std::unexpected(StateError(std::string_view("expected ':' after static field name")));
    }
    ts.Consume();

    SkipTrivia(ts);
    auto type = ctx.TypeParser()->ParseType(ts, *ctx.Diags());
    if (type == nullptr) {
      return std::unexpected(StateError(std::string_view("failed to parse static field type")));
    }

    std::unique_ptr<Expr> init = nullptr;
    SkipTrivia(ts);
    if (!ts.IsEof() && ts.Peek().GetLexeme() == "=") {
      ts.Consume();
      SkipTrivia(ts);
      init = ctx.Expr()->Parse(ts, *ctx.Diags());
    }

    span = Union(span, ts.LastConsumed() ? SpanFrom(*ts.LastConsumed()) : span);
    auto field =
        ctx.Factory()->MakeStaticField(is_public, is_var, std::move(name), std::move(*type), std::move(init), span);
    class_decl->AddMember(std::move(field));
    ConsumeTerminators(ts);
    return false;
  }

  // Check for destructor
  if (lex == "destructor") {
    ctx.PopState();
    ctx.PushState(StateRegistry::DestructorDecl());
    return true;
  }

  // Check for call
  if (lex == "call") {
    ctx.PushState(StateRegistry::CallDeclHdr());
    return true;
  }

  // Check for method (fun)
  if (lex == "override") {
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      return std::unexpected(StateError(std::string_view("unexpected end of file after 'override'")));
    }
    lex = ts.Peek().GetLexeme();
  }
  if (lex == "pure") {
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof() || ts.Peek().GetLexeme() != "fun") {
      return std::unexpected(StateError(std::string_view("expected 'fun' after 'pure'")));
    }
  }

  if (lex == "fun") {
    ts.Rewind(ts.Position() - start_index);
    ctx.PopState();
    ctx.PushState(StateRegistry::MethodHdr());
    return true;
  }

  // Field declaration
  bool is_var = false;
  if (lex == "var" || lex == "val") {
    is_var = lex == "var";
    ts.Consume();
    SkipTrivia(ts);
  } else {
    return std::unexpected(StateError(std::string_view("expected class member")));
  }

  std::string name = ReadIdentifier(ctx, ts, "P_FIELD_NAME", "expected field name");
  if (name.empty()) {
    return std::unexpected(StateError(std::string_view("expected field name")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != ":") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FIELD_COLON", std::string_view("expected ':' after field name"));
    }
    return std::unexpected(StateError(std::string_view("expected ':' after field name")));
  }
  ts.Consume();

  SkipTrivia(ts);
  auto type = ctx.TypeParser()->ParseType(ts, *ctx.Diags());
  if (type == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse field type")));
  }

  std::unique_ptr<Expr> init = nullptr;
  SkipTrivia(ts);
  if (!ts.IsEof() && ts.Peek().GetLexeme() == "=") {
    ts.Consume();
    SkipTrivia(ts);
    init = ctx.Expr()->Parse(ts, *ctx.Diags());
  }

  span = Union(span, ts.LastConsumed() ? SpanFrom(*ts.LastConsumed()) : span);
  auto field = ctx.Factory()->MakeField(is_public, is_var, std::move(name), std::move(*type), std::move(init), span);
  class_decl->AddMember(std::move(field));
  ConsumeTerminators(ts);
  return false;
}

} // namespace ovum::compiler::parser
