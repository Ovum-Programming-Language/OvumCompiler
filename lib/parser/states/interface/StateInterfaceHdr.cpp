#include "StateInterfaceHdr.hpp"

#include <memory>
#include <string>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"

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
  const MatchIdentifier matcher;
  return matcher.TryMatch(token);
}

std::string ReadIdentifier(const ContextParser& ctx, ITokenStream& ts) {
  SkipTrivia(ts);
  if (ts.IsEof() || !IsIdentifier(ts.Peek())) {
    if (ctx.Diags() != nullptr) {
      if (const Token* tok = ts.TryPeek(); tok != nullptr) {
        SourceSpan span = StateBase::SpanFrom(*tok);
        ctx.Diags()->Error("P_INTERFACE_NAME", "expected interface name", span);
      } else {
        ctx.Diags()->Error("P_INTERFACE_NAME", "expected interface name");
      }
    }
    return "";
  }
  std::string name = ts.Consume()->GetLexeme();
  return name;
}

} // namespace

std::string_view StateInterfaceHdr::Name() const {
  return "InterfaceHdr";
}

IState::StepResult StateInterfaceHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in interface header")));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "interface") {
    return std::unexpected(StateError(std::string_view("expected 'interface' keyword")));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts);
  if (name.empty()) {
    return std::unexpected(StateError(std::string_view("expected interface name")));
  }

  const SourceSpan span = SpanFrom(start);
  auto interface_decl = ctx.Factory()->MakeInterface(std::move(name), {}, span);
  ctx.PushNode(std::move(interface_decl));

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_INTERFACE_BODY", "expected '{' for interface body");
    }
    return std::unexpected(StateError(std::string_view("expected '{' for interface body")));
  }
  ts.Consume();
  ctx.PopState();
  ctx.PushState(StateRegistry::InterfaceBody());
  return true;
}

} // namespace ovum::compiler::parser
