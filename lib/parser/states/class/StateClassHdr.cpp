#include "StateClassHdr.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "type_parser/ITypeParser.hpp"

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

bool IsIdentifier(const Token& token) {
  MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(const ContextParser& ctx, ITokenStream& ts) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error("P_CLASS_NAME", "expected class name", span);
      } else {
        ctx.Diags()->Error("P_CLASS_NAME", "expected class name");
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

std::unique_ptr<TypeReference> ParseType(const ContextParser& ctx, ITokenStream& ts) {
  if (ctx.TypeParser() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPE_PARSER", "type parser not available");
    }
    return nullptr;
  }
  return ctx.TypeParser()->ParseType(ts, *ctx.Diags());
}

} // namespace

std::string_view StateClassHdr::Name() const {
  return "ClassHdr";
}

IState::StepResult StateClassHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in class header")));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "class") {
    return std::unexpected(StateError(std::string_view("expected 'class' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts);
  if (name.empty()) {
    return std::unexpected(StateError(std::string_view("expected class name")));
  }

  SourceSpan span = SpanFrom(start);

  // Check for implements clause
  std::vector<TypeReference> implements;
  SkipTrivia(ts);
  if (!ts.IsEof() && ts.Peek().GetLexeme() == "implements") {
    ts.Consume();
    SkipTrivia(ts);

    while (true) {
      auto type = ParseType(ctx, ts);
      if (type == nullptr) {
        break;
      }
      implements.push_back(std::move(*type));

      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != ",") {
        break;
      }
      ts.Consume();
      SkipTrivia(ts);
    }
  }

  auto class_decl = ctx.Factory()->MakeClass(std::move(name), std::move(implements), {}, span);
  ctx.PushNode(std::move(class_decl));

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_CLASS_BODY", "expected '{' for class body");
    }
    return std::unexpected(StateError(std::string_view("expected '{' for class body")));
  }

  ts.Consume();

  ctx.PopState();
  ctx.PushState(StateRegistry::ClassBody());
  return true;
}

} // namespace ovum::compiler::parser
