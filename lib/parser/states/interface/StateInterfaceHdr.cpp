#include "StateInterfaceHdr.hpp"

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
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

}  // namespace

std::string_view StateInterfaceHdr::Name() const {
  return "InterfaceHdr";
}

IState::StepResult StateInterfaceHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in interface header"));
  }

  const Token& start = ts.Peek();
  if (start.GetLexeme() != "interface") {
    return std::unexpected(StateError("expected 'interface' keyword"));
  }
  ts.Consume();

  SkipTrivia(ts);
  std::string name = ReadIdentifier(ctx, ts, "P_INTERFACE_NAME", "expected interface name");
  if (name.empty()) {
    return std::unexpected(StateError("expected interface name"));
  }

  SourceSpan span = StateBase::SpanFrom(start);
  auto interface_decl = ctx.Factory()->MakeInterface(std::move(name), {}, span);
  ctx.PushNode(std::unique_ptr<AstNode>(interface_decl.get()));

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_INTERFACE_BODY", "expected '{' for interface body");
    }
    return std::unexpected(StateError("expected '{' for interface body"));
  }

  ctx.PushState(StateRegistry::InterfaceBody());
  return true;
}

}  // namespace ovum::compiler::parser
