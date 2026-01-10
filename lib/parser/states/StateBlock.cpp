#include "StateBlock.hpp"

#include <memory>

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
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

} // namespace

std::string_view StateBlock::Name() const {
  return "Block";
}

IState::StepResult StateBlock::TryStep(ContextParser& ctx, ITokenStream& ts) const {
  SkipTrivia(ts);

  if (ts.IsEof()) {
    return std::unexpected(StateError(std::string_view("unexpected end of file in block")));
  }

  Block* block = ctx.TopNodeAs<Block>();
  if (block == nullptr) {
    return std::unexpected(StateError(std::string_view("expected Block node on stack")));
  }

  const Token& tok = ts.Peek();
  if (tok.GetLexeme() == "}") {
    ts.Consume();

    // Check if there's a parent node that needs this block
    if (ctx.NodeStack().size() >= 2) {
      auto parent_node = ctx.NodeStack()[ctx.NodeStack().size() - 2].MutableNode();

      // Check if parent is FunctionDecl
      auto* func = dynamic_cast<FunctionDecl*>(parent_node);
      if (func != nullptr && func->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* block = dynamic_cast<Block*>(block_node.get());
        if (block != nullptr) {
          func->SetBody(std::unique_ptr<Block>(block));
          block_node.release();

          // Pop function and add to module
          auto func_node = ctx.PopNode();
          auto* module = ctx.TopNodeAs<Module>();
          if (module != nullptr) {
            module->AddDecl(std::unique_ptr<Decl>(dynamic_cast<Decl*>(func_node.release())));
          }
        }
        return false;
      }

      // Check if parent is IfStmt (for else block)
      auto* if_stmt = dynamic_cast<IfStmt*>(parent_node);
      if (if_stmt != nullptr && if_stmt->ElseBlock() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* else_block = dynamic_cast<Block*>(block_node.get());
        if (else_block != nullptr) {
          if_stmt->SetElseBlock(std::unique_ptr<Block>(else_block));
          block_node.release();

          // Pop IfStmt and add to parent block
          auto if_node = ctx.PopNode();
          Block* parent_block = ctx.TopNodeAs<Block>();
          if (parent_block != nullptr) {
            parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(if_node.release())));
          }
        }
        return false;
      }

      // Check if parent is WhileStmt
      auto* while_stmt = dynamic_cast<WhileStmt*>(parent_node);
      if (while_stmt != nullptr && while_stmt->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          while_stmt->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();
        }
        auto while_node = ctx.PopNode();
        Block* parent_block = ctx.TopNodeAs<Block>();
        if (parent_block != nullptr) {
          parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(while_node.release())));
        }
        // Don't call PopState here - ParserFsm will do it when returning false
        return false;
      }

      // Check if parent is ForStmt
      auto* for_stmt = dynamic_cast<ForStmt*>(parent_node);
      if (for_stmt != nullptr && for_stmt->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          for_stmt->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();
        }
        // Pop ForStmt and add to parent block
        auto for_node = ctx.PopNode();
        Block* parent_block = ctx.TopNodeAs<Block>();
        if (parent_block != nullptr) {
          parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(for_node.release())));
        }
        // Don't call PopState here - ParserFsm will do it when returning false
        return false;
      }

      // Check if parent is UnsafeBlock
      auto* unsafe_stmt = dynamic_cast<UnsafeBlock*>(parent_node);
      if (unsafe_stmt != nullptr && unsafe_stmt->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          unsafe_stmt->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();
        }
        // Pop UnsafeBlock and add to parent block
        auto unsafe_node = ctx.PopNode();
        Block* parent_block = ctx.TopNodeAs<Block>();
        if (parent_block != nullptr) {
          parent_block->Append(std::unique_ptr<Stmt>(dynamic_cast<Stmt*>(unsafe_node.release())));
        }
        // Don't call PopState here - ParserFsm will do it when returning false
        return false;
      }

      // Check if parent is MethodDecl
      auto* method = dynamic_cast<MethodDecl*>(parent_node);
      if (method != nullptr && method->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          method->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();

          // Pop method and add to class
          auto method_node = ctx.PopNode();
          auto* class_decl = ctx.TopNodeAs<ClassDecl>();
          if (class_decl != nullptr) {
            class_decl->AddMember(std::unique_ptr<Decl>(dynamic_cast<Decl*>(method_node.release())));
          }
        }
        return false;
      }

      // Check if parent is CallDecl
      auto* call = dynamic_cast<CallDecl*>(parent_node);
      if (call != nullptr && call->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          call->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();

          // Pop call and add to class
          auto call_node = ctx.PopNode();
          auto* class_decl = ctx.TopNodeAs<ClassDecl>();
          if (class_decl != nullptr) {
            class_decl->AddMember(std::unique_ptr<Decl>(dynamic_cast<Decl*>(call_node.release())));
          }
        }
        return false;
      }

      // Check if parent is DestructorDecl
      auto* destructor = dynamic_cast<DestructorDecl*>(parent_node);
      if (destructor != nullptr && destructor->Body() == nullptr) {
        auto block_node = ctx.PopNode();
        auto* body_block = dynamic_cast<Block*>(block_node.get());
        if (body_block != nullptr) {
          destructor->SetBody(std::unique_ptr<Block>(body_block));
          block_node.release();

          // Pop destructor and add to class
          auto destructor_node = ctx.PopNode();
          ClassDecl* class_decl = ctx.TopNodeAs<ClassDecl>();
          if (class_decl != nullptr) {
            class_decl->AddMember(std::unique_ptr<Decl>(dynamic_cast<Decl*>(destructor_node.release())));
          }
        }
        ctx.PopState();
        return false;
      }
    }

    return false; // Block complete
  }

  ctx.PushState(StateRegistry::Stmt());
  return true;
}

} // namespace ovum::compiler::parser
