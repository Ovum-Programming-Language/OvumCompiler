#include "StateTypeAliasDecl.hpp"

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
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

void ConsumeTerminators(ITokenStream& ts) {
  SkipTrivia(ts, false);
  while (!ts.IsEof()) {
    const Token& t = ts.Peek();
    const std::string type = t.GetStringType();
    if (type == "NEWLINE") {
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

}  // namespace

std::string_view StateTypeAliasDecl::Name() const {
  return "TypeAliasDecl";
}

IState::StepResult StateTypeAliasDecl::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  Module* module = ctx.TopNodeAs<Module>();
  if (module == nullptr) {
    return std::unexpected(StateError("expected Module node on stack"));
  }

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in type alias"));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "typealias") {
    return std::unexpected(StateError("expected 'typealias' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts, "P_TYPEALIAS_NAME", "expected type alias name");
  if (name.empty()) {
    return std::unexpected(StateError("expected type alias name"));
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "=") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPEALIAS_EQ", "expected '=' after type alias name");
    }
    return std::unexpected(StateError("expected '=' after type alias name"));
  }
  ts.Consume();

  SkipTrivia(ts);
  auto aliased_type = ctx.TypeParser()->ParseType(ts, *ctx.Diags());
  if (aliased_type == nullptr) {
    return std::unexpected(StateError("failed to parse aliased type"));
  }

  SourceSpan span = StateBase::SpanFrom(start);
  if (ts.LastConsumed() != nullptr) {
    span = StateBase::Union(span, StateBase::SpanFrom(*ts.LastConsumed()));
  }
  auto type_alias = ctx.Factory()->MakeTypeAlias(std::move(name), std::move(*aliased_type), span);
  module->AddDecl(std::move(type_alias));

  ConsumeTerminators(ts);
  return false;
}

}  // namespace ovum::compiler::parser
