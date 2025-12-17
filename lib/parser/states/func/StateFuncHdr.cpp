#include "StateFuncHdr.hpp"

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"

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

std::string_view StateFuncHdr::Name() const {
  return "FuncHdr";
}

IState::StepResult StateFuncHdr::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError("unexpected end of file in function header"));
  }

  // Check if we already have a function node
  FunctionDecl* func = ctx.TopNodeAs<FunctionDecl>();
  if (func == nullptr) {
    // Create new function node
    bool is_pure = false;
    const Token& start = ts.Peek();
    
    // Check for 'pure' keyword (should have been consumed by StateTopDecl)
    // But we handle it here too for safety
    if (start.GetLexeme() == "pure") {
      ts.Consume();
      is_pure = true;
      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != "fun") {
        if (ctx.Diags() != nullptr) {
          ctx.Diags()->Error("P_PURE_FUN", "expected 'fun' after 'pure'");
        }
        return std::unexpected(StateError("expected 'fun' after 'pure'"));
      }
    }

    if (ts.Peek().GetLexeme() != "fun") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_FUN_KEYWORD", "expected 'fun' keyword");
      }
      return std::unexpected(StateError("expected 'fun' keyword"));
    }
    ts.Consume();

    SkipTrivia(ts);
    std::string name = ReadIdentifier(ctx, ts, "P_FUN_NAME", "expected function name");
    if (name.empty()) {
      return std::unexpected(StateError("expected function name"));
    }

    SourceSpan span = StateBase::SpanFrom(start);
    auto func_node = ctx.Factory()->MakeFunction(is_pure, std::move(name), {}, nullptr, nullptr,
                                                 span);
    ctx.PushNode(std::unique_ptr<AstNode>(func_node.get()));
    func = func_node.get();
  }

  SkipTrivia(ts);
  if (ts.IsEof() || ts.Peek().GetLexeme() != "(") {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_FUN_PARAMS_OPEN", "expected '(' after function name");
    }
    return std::unexpected(StateError("expected '(' after function name"));
  }

  ctx.PushState(StateRegistry::FuncParams());
  return true;
}

}  // namespace ovum::compiler::parser
