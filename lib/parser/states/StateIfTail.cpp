#include "StateIfTail.hpp"

#include <memory>

#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/Branch.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

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

}  // namespace

std::string_view StateIfTail::Name() const {
  return "IfTail";
}

IState::StepResult StateIfTail::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  // Stack should have: [IfStmt, condition Expr, then Block]
  if (ctx.NodeStack().size() < 3) {
    return std::unexpected(StateError("expected IfStmt, condition, and block on stack"));
  }

  // Pop then block
  auto then_block_node = ctx.PopNode();
  Block* then_block = dynamic_cast<Block*>(then_block_node.get());
  if (then_block == nullptr) {
    return std::unexpected(StateError("expected Block on stack"));
  }

  // Pop condition
  auto condition_node = ctx.PopNode();
  Expr* condition = dynamic_cast<Expr*>(condition_node.get());
  if (condition == nullptr) {
    return std::unexpected(StateError("expected Expr on stack"));
  }

  // Get IfStmt
  IfStmt* if_stmt = ctx.TopNodeAs<IfStmt>();
  if (if_stmt == nullptr) {
    return std::unexpected(StateError("expected IfStmt on stack"));
  }

  // Add branch
  Branch branch(std::unique_ptr<Expr>(condition), std::unique_ptr<Block>(then_block));
  condition_node.release();
  then_block_node.release();
  if_stmt->AddBranch(std::move(branch));

  // Check for else if or else
  if (ts.IsEof()) {
    // IfStmt complete, add to parent block
    auto if_node = ctx.PopNode();
    Block* parent_block = ctx.TopNodeAs<Block>();
    if (parent_block != nullptr) {
      parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(if_node.release())));
    }
    return false;
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == "else") {
    ts.Consume();
    SkipTrivia(ts);
    
    if (!ts.IsEof() && ts.Peek().GetLexeme() == "if") {
      // else if - create new branch
      ctx.PushState(StateRegistry::IfHead());
      return true;
    }
    
    // else block
    if (ts.IsEof() || ts.Peek().GetLexeme() != "{") {
      if (ctx.Diags() != nullptr) {
        ctx.Diags()->Error("P_ELSE_BLOCK", "expected '{' for else body");
      }
      return std::unexpected(StateError("expected '{' for else body"));
    }
    
    ts.Consume();
    auto else_block = ctx.Factory()->MakeBlock({}, SourceSpan{});
    ctx.PushNode(std::unique_ptr<AstNode>(else_block.get()));
    ctx.PushState(StateRegistry::Block());
    
    // IfStmt will be handled when else block completes
    return true;
  }

  // IfStmt complete, add to parent block
  auto if_node = ctx.PopNode();
  Block* parent_block = ctx.TopNodeAs<Block>();
  if (parent_block != nullptr) {
    parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(if_node.release())));
  }
  return false;
}

}  // namespace ovum::compiler::parser
