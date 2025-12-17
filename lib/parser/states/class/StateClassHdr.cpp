#include "StateClassHdr.hpp"

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
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

std::unique_ptr<TypeReference> ParseType(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.TypeParser() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_TYPE_PARSER", "type parser not available");
    }
    return nullptr;
  }
  return ctx.TypeParser()->ParseType(ts, *ctx.Diags());
}

}  // namespace

std::string_view StateClassHdr::Name() const {
  return "ClassHdr";
}

IState::StepResult StateClassHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in class header"));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "class") {
    return std::unexpected(StateError("expected 'class' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts, "P_CLASS_NAME", "expected class name");
  if (name.empty()) {
    return std::unexpected(StateError("expected class name"));
  }

  SourceSpan span = StateBase::SpanFrom(start);

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
  ctx.PushNode(std::unique_ptr<AstNode>(class_decl.get()));

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_CLASS_BODY", "expected '{' for class body");
    }
    return std::unexpected(StateError("expected '{' for class body"));
  }

  ctx.PushState(StateRegistry::ClassBody());
  return true;
}

}  // namespace ovum::compiler::parser
