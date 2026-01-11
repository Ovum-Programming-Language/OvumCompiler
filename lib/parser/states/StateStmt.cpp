#include "StateStmt.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/tokens/token_traits/MatchIdentifier.hpp"
#include "lib/parser/tokens/token_traits/MatchLexeme.hpp"
#include "pratt/IExpressionParser.hpp"
#include "type_parser/ITypeParser.hpp"
#include "types/TypeReference.hpp"

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

void ReportUnexpected(IDiagnosticSink* diags, std::string_view code, std::string_view message, const Token* tok) {
  if (diags == nullptr || tok == nullptr) {
    return;
  }
  SourceSpan span = StateBase::SpanFrom(*tok);
  diags->Error(code, message, span);
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

std::unique_ptr<Expr> ParseExpression(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.Expr() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_EXPR_PARSER", "expression parser not available");
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
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

std::string_view StateStmt::Name() const {
  return "Stmt";
}

IState::StepResult StateStmt::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return false;
  }

  auto* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  const Token& tok = ts.Peek();
  std::string lex = tok.GetLexeme();

  if (lex == "if") {
    ctx.PopState();
    ctx.PushState(StateRegistry::IfHead());
    return true;
  }

  if (lex == "while") {
    ctx.PopState();
    ctx.PushState(StateRegistry::WhileHead());
    return true;
  }

  if (lex == "for") {
    ctx.PopState();
    ctx.PushState(StateRegistry::ForHead());
    return true;
  }

  if (lex == "return") {
    ctx.PopState();
    ctx.PushState(StateRegistry::ReturnTail());
    ts.Consume();
    return true;
  }

  if (lex == "break") {
    ts.Consume();
    SourceSpan span = SpanFrom(tok);
    auto stmt = ctx.Factory()->MakeBreakStmt(span);
    block->Append(std::move(stmt));
    ConsumeTerminators(ts);
    return false;
  }

  if (lex == "continue") {
    ts.Consume();
    SourceSpan span = SpanFrom(tok);
    auto stmt = ctx.Factory()->MakeContinueStmt(span);
    block->Append(std::move(stmt));
    ConsumeTerminators(ts);
    return false;
  }

  if (lex == "unsafe") {
    ctx.PopState();
    ctx.PushState(StateRegistry::UnsafeBlock());
    ts.Consume();
    return true;
  }

  if (lex == "{") {
    ctx.PopState();
    ctx.PushState(StateRegistry::Block());
    ts.Consume();
    return true;
  }

  // Try variable declaration: [var] identifier : type = expression
  bool is_var = false;
  if (lex == "var" || lex == "val") {
    is_var = lex == "var";
    ts.Consume();
    SkipTrivia(ts);
    if (ts.IsEof()) {
      ReportUnexpected(ctx.Diags(), "P_VAR_NAME", "expected variable name", ts.TryPeek());
      return std::unexpected(StateError(std::string_view("expected variable name")));
    }
  }

  if (IsIdentifier(ts.Peek())) {
    std::string name = ts.Consume()->GetLexeme();
    SkipTrivia(ts);

    if (!ts.IsEof() && ts.Peek().GetLexeme() == ":") {
      // Variable declaration
      ts.Consume();
      SkipTrivia(ts);

      auto type = ParseType(ctx, ts);
      if (type == nullptr) {
        return std::unexpected(StateError(std::string_view("failed to parse type")));
      }

      SkipTrivia(ts);
      if (ts.IsEof() || ts.Peek().GetLexeme() != "=") {
        ReportUnexpected(ctx.Diags(), "P_VAR_INIT", "expected '=' for variable", ts.TryPeek());
        return std::unexpected(StateError(std::string_view("expected '=' for variable")));
      }
      ts.Consume();

      SkipTrivia(ts);
      auto init = ParseExpression(ctx, ts);
      if (init == nullptr) {
        return std::unexpected(StateError(std::string_view("failed to parse initialization expression")));
      }

      SourceSpan span = Union(SpanFrom(tok), init->Span());
      auto stmt = ctx.Factory()->MakeVarDeclStmt(is_var, std::move(name), std::move(*type), std::move(init), span);
      block->Append(std::move(stmt));
      ConsumeTerminators(ts);
      return false;
    }
    ts.Rewind(1);
  }

  // Expression statement
  auto expr = ParseExpression(ctx, ts);
  if (expr == nullptr) {
    return std::unexpected(StateError(std::string_view("failed to parse expression")));
  }

  SourceSpan span = expr->Span();
  auto stmt = ctx.Factory()->MakeExprStmt(std::move(expr), span);
  block->Append(std::move(stmt));
  ConsumeTerminators(ts);
  return false;
}

} // namespace ovum::compiler::parser
