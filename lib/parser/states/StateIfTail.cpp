#include "StateIfTail.hpp"

#include <memory>

#include "ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/Branch.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "pratt/IExpressionParser.hpp"

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

std::unique_ptr<Expr> ParseExpression(ContextParser& ctx, ITokenStream& ts) {
  if (ctx.Expr() == nullptr) {
    if (ctx.Diags() != nullptr) {
      ctx.Diags()->Error("P_EXPR_PARSER", "expression parser not available");
    }
    return nullptr;
  }
  return ctx.Expr()->Parse(ts, *ctx.Diags());
}

} // namespace

std::string_view StateIfTail::Name() const {
  return "IfTail";
}

IState::StepResult StateIfTail::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  IfStmt* if_stmt = nullptr;
  for (auto it = ctx.NodeStack().rbegin(); it != ctx.NodeStack().rend(); ++it) {
    if_stmt = dynamic_cast<IfStmt*>(it->MutableNode());
    if (if_stmt) {
      break;
    }
  }

  if (!if_stmt) {
    return std::unexpected(StateError(std::string_view("expected IfStmt somewhere in stack")));
  }

  auto& branches = if_stmt->MutableBranches();
  Branch* last_branch = branches.empty() ? nullptr : &branches.back();

  if (!last_branch || last_branch->Condition() == nullptr) {
    if (ctx.NodeStack().size() < 2) {
      return std::unexpected(StateError(std::string_view("expected condition and then block on stack")));
    }

    auto then_block_node = ctx.PopNode();
    auto condition_node = ctx.PopNode();

    auto then_block = std::unique_ptr<Block>(dynamic_cast<Block*>(then_block_node.release()));
    auto condition = std::unique_ptr<Expr>(dynamic_cast<Expr*>(condition_node.release()));

    if (!then_block || !condition) {
      return std::unexpected(StateError(std::string_view("invalid if branch components")));
    }

    if (!last_branch) {
      if_stmt->AddBranch(Branch(std::move(condition), std::move(then_block)));
      last_branch = &branches.back();
    } else {
      last_branch->SetCondition(std::move(condition));
      last_branch->SetThen(std::move(then_block));
    }
  }

  if (!ts.IsEof() && ts.Peek().GetLexeme() == "else") {
    ts.Consume();
    SkipTrivia(ts);

    if (!ts.IsEof() && ts.Peek().GetLexeme() == "if") {
      // else if
      ctx.PushState(StateRegistry::IfHead());
      return true;
    }

    // else block
    if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
      if (ctx.Diags()) {
        ctx.Diags()->Error("P_ELSE_BLOCK", "expected '{' for else body");
      }
      return std::unexpected(StateError(std::string_view("expected '{' for else body")));
    }

    ts.Consume();
    ctx.PopState();
    auto else_block = ctx.Factory()->MakeBlock({}, SourceSpan{});
    ctx.PushNode(std::move(else_block));
    ctx.PushState(StateRegistry::Block());

    return true;
  }

  auto if_node = ctx.PopNode();
  if (auto* parent_block = ctx.TopNodeAs<Block>()) {
    parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(if_node.release())));
  }

  return false;
}

} // namespace ovum::compiler::parser
