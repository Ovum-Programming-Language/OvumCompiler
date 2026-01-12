#include "StateTypeAliasDecl.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "type_parser/ITypeParser.hpp"

namespace ovum::compiler::parser {

namespace {

void SkipTrivia(ITokenStream& ts, const bool skip_newlines = true) {
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
  const MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(const ContextParser& ctx, ITokenStream& ts) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error("P_TYPEALIAS_NAME", "expected type alias name", span);
      } else {
        ctx.Diags()->Error("P_TYPEALIAS_NAME", "expected type alias name");
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

std::string_view StateTypeAliasDecl::Name() const {
  return "TypeAliasDecl";
}

IState::StepResult StateTypeAliasDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  auto* module = ctx.TopNodeAs<Module>();
  if (module == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Module node on stack")));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in type alias")));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "typealias") {
    return std::unexpected(StateError(std::string_view("expected 'typealias' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts);
  if (name.empty()) {
    return std::unexpected(StateError(std::string_view("expected type alias name")));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "=") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPEALIAS_EQ", std::string_view("expected '=' after type alias name"));
    }
    return std::unexpected(StateError(std::string_view("expected '=' after type alias name")));
  }
  ts.Consume();

  SkipTrivia(ts);
  const auto aliased_type = ctx.TypeParser()->ParseType(ts, *ctx.Diags());
  if (aliased_type == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse aliased type")));
  }

  SourceSpan span = SpanFrom(start);
  if (ts.LastConsumed() != nullptr) {
    span = Union(span, SpanFrom(*ts.LastConsumed()));
  }
  auto type_alias = ctx.Factory()->MakeTypeAlias(std::move(name), std::move(*aliased_type), span);
  module->AddDecl(std::move(type_alias));

  ConsumeTerminators(ts);
  return false;
}

} // namespace ovum::compiler::parser
