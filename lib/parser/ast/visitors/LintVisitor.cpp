#include "LintVisitor.hpp"

#include <vector>

#include "lib/parser/diagnostics/severity/Severity.hpp"

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"

#include "lib/parser/ast/nodes/exprs/Assign.hpp"
#include "lib/parser/ast/nodes/exprs/Call.hpp"
#include "lib/parser/ast/nodes/exprs/SafeCall.hpp"

#include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"

#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"

void LintVisitor::EnterBody() {
  ++nesting_depth_;
}

void LintVisitor::LeaveBody() {
  if (nesting_depth_ > 0) {
    --nesting_depth_;
  }
}

void LintVisitor::EnterLoop() {
  ++loop_depth_;
}

void LintVisitor::LeaveLoop() {
  if (loop_depth_ > 0) {
    --loop_depth_;
  }
}

void LintVisitor::CheckNestingDepth(SourceSpan) const {
  if (opts_.warn_deep_nesting && nesting_depth_ > opts_.max_nesting) {
    sink_.Warn("W0201", "deep nesting");
  }
}

bool LintVisitor::IsPureExpr(Expr& expression) const {
  if (dynamic_cast<Assign*>(&expression)) {
    return false;
  }

  if (dynamic_cast<Call*>(&expression)) {
    return false;
  }

  if (dynamic_cast<SafeCall*>(&expression)) {
    return false;
  }

  return true;
}

void LintVisitor::Visit(Module& node) {
  if (opts_.warn_module_without_decls && node.MutableDecls().empty()) {
    sink_.Warn("W0001", "module has no declarations");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(ClassDecl& node) {
  if (opts_.warn_large_class && node.MutableMembers().size() > opts_.max_class_members) {
    sink_.Warn("W0101", "class has too many members");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(FunctionDecl& node) {
  if (opts_.warn_empty_bodies) {
    if (auto* b = node.MutableBody()) {
      if (b->Size() == 0) {
        sink_.Warn("W0102", "function body is empty");
      }
    }
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(MethodDecl& node) {
  if (opts_.warn_empty_bodies) {
    if (!node.IsPure()) {
      if (auto* b = node.MutableBody()) {
        if (b->Size() == 0) {
          sink_.Warn("W0103", "method body is empty");
        }
      }
    }
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(CallDecl& node) {
  if (opts_.warn_empty_bodies) {
    if (auto* b = node.MutableBody()) {
      if (b->Size() == 0) {
        sink_.Warn("W0104", "call body is empty");
      }
    }
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(DestructorDecl& node) {
  if (opts_.warn_empty_bodies) {
    if (auto* b = node.MutableBody()) {
      if (b->Size() == 0) {
        sink_.Warn("W0105", "destructor body is empty");
      }
    }
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(Block& node) {
  EnterBody();

  if (opts_.warn_empty_blocks && node.GetStatements().empty()) {
    sink_.Warn("W0202", "empty block");
  }

  CheckNestingDepth();

  bool terminated = false;
  if (opts_.warn_unreachable) {
    for (const auto& stmt : node.GetStatements()) {
      if (terminated) {
        sink_.Warn("W0301", "unreachable statement");
        continue;
      }

      if (dynamic_cast<ReturnStmt*>(stmt.get())) {
        terminated = true;
      } else if (dynamic_cast<BreakStmt*>(stmt.get())) {
        terminated = true;
      } else if (dynamic_cast<ContinueStmt*>(stmt.get())) {
        terminated = true;
      }
    }
  }

  if (opts_.max_block_len > 0 && node.GetStatements().size() > opts_.max_block_len) {
    sink_.Warn("W0203", "block is too long");
  }

  for (auto& stmt : node.GetStatements()) {
    stmt->Accept(*this);
  }

  LeaveBody();
}

void LintVisitor::Visit(ExprStmt& node) {
  if (opts_.warn_pure_expr_stmt) {
    if (auto* e = node.MutableExpression()) {
      if (IsPureExpr(*e)) {
        sink_.Warn("W0401", "expression statement has no effect");
      }
    }
  }
  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(ReturnStmt& node) {
  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(BreakStmt& node) {
  if (opts_.warn_break_continue_outside_loop && loop_depth_ == 0) {
    sink_.Error("E0301", "break outside of loop");
  }
  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(ContinueStmt& node) {
  if (opts_.warn_break_continue_outside_loop && loop_depth_ == 0) {
    sink_.Error("E0302", "continue outside of loop");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(IfStmt& node) {
  EnterBody();

  if (opts_.warn_if_without_branches && node.MutableBranches().empty() && !node.HasElse()) {
    sink_.Warn("W0501", "if statement has no branches");
  }

  for (auto& br : node.MutableBranches()) {
    if (auto* then_blk = br.MutableThen()) {
      if (opts_.warn_empty_blocks && then_blk->GetStatements().empty()) {
        sink_.Warn("W0502", "then-branch is empty");
      }
    }
  }

  if (opts_.warn_empty_else && node.HasElse()) {
    if (auto* eb = node.MutableElseBlock()) {
      if (eb->GetStatements().empty()) {
        sink_.Warn("W0503", "else-branch is empty");
      }
    }
  }

  WalkVisitor::Visit(node);

  LeaveBody();
}

void LintVisitor::Visit(WhileStmt& node) {
  EnterBody();

  if (opts_.warn_missing_loop_cond_or_iterable && node.MutableCondition() == nullptr) {
    sink_.Error("E0401", "while loop without condition");
  }

  if (opts_.warn_while_true) {
    if (auto* cond = node.MutableCondition()) {
      if (auto* bl = dynamic_cast<BoolLit*>(cond)) {
        if (bl->Value()) {
          sink_.Warn("W0601", "while(true) loop");
        }
      }
    }
  }

  EnterLoop();
  WalkVisitor::Visit(node);
  LeaveLoop();

  LeaveBody();
}

void LintVisitor::Visit(ForStmt& node) {
  EnterBody();

  if (opts_.warn_missing_loop_cond_or_iterable && node.MutableIteratorExpr() == nullptr) {
    sink_.Error("E0402", "for loop without iterable expression");
  }

  EnterLoop();
  WalkVisitor::Visit(node);
  LeaveLoop();

  LeaveBody();
}

void LintVisitor::Visit(UnsafeBlock& node) {
  EnterBody();
  if (opts_.warn_empty_blocks) {
    if (auto* b = node.MutableBody()) {
      if (b->GetStatements().empty()) {
        sink_.Warn("W0701", "empty unsafe block");
      }
    }
  }

  WalkVisitor::Visit(node);
  LeaveBody();
}

void LintVisitor::Visit(GlobalVarDecl& node) {
  if (opts_.warn_mutable_globals && node.IsVar()) {
    sink_.Warn("W0801", "mutable global variable");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(FieldDecl& node) {
  if (opts_.warn_public_fields && node.IsPublic()) {
    sink_.Warn("W0802", "public field");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(StaticFieldDecl& node) {
  if (opts_.warn_static_mutable_fields && node.IsVar()) {
    sink_.Warn("W0803", "static mutable field");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(StringLit& node) {
  if (opts_.warn_empty_string_literal && node.Value().empty()) {
    sink_.Warn("W0901", "empty string literal");
  }

  WalkVisitor::Visit(node);
}

void LintVisitor::Visit(BoolLit& node) {
  (void) node;
  WalkVisitor::Visit(node);
}
