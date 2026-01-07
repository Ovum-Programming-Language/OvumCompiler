#include "WalkVisitor.hpp"

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"

#include "lib/parser/ast/nodes/exprs/Assign.hpp"
#include "lib/parser/ast/nodes/exprs/Binary.hpp"
#include "lib/parser/ast/nodes/exprs/Call.hpp"
#include "lib/parser/ast/nodes/exprs/CastAs.hpp"
#include "lib/parser/ast/nodes/exprs/Elvis.hpp"
#include "lib/parser/ast/nodes/exprs/FieldAccess.hpp"
#include "lib/parser/ast/nodes/exprs/IdentRef.hpp"
#include "lib/parser/ast/nodes/exprs/IndexAccess.hpp"
#include "lib/parser/ast/nodes/exprs/NamespaceRef.hpp"
#include "lib/parser/ast/nodes/exprs/SafeCall.hpp"
#include "lib/parser/ast/nodes/exprs/TypeTestIs.hpp"
#include "lib/parser/ast/nodes/exprs/Unary.hpp"

#include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/CharLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/FloatLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/IntLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/NullLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"

#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
#include "lib/parser/ast/nodes/stmts/VarDeclStmt.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"

namespace ovum::compiler::parser {

void WalkVisitor::Visit(Module& node) {
  for (auto& decl_ptr : node.MutableDecls()) {
    decl_ptr->Accept(*this);
  }
}

void WalkVisitor::Visit(FunctionDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void WalkVisitor::Visit(ClassDecl& node) {
  for (auto& member : node.MutableMembers()) {
    member->Accept(*this);
  }
}

void WalkVisitor::Visit(InterfaceMethod& node) {
  (void) node;
}

void WalkVisitor::Visit(InterfaceDecl& node) {
  for (auto& m : node.MutableMembers()) {
    m->Accept(*this);
  }
}

void WalkVisitor::Visit(TypeAliasDecl& node) {
  (void) node;
}

void WalkVisitor::Visit(GlobalVarDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void WalkVisitor::Visit(FieldDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void WalkVisitor::Visit(StaticFieldDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void WalkVisitor::Visit(MethodDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void WalkVisitor::Visit(CallDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void WalkVisitor::Visit(DestructorDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void WalkVisitor::Visit(Block& node) {
  for (auto& stmt : node.GetStatements()) {
    stmt->Accept(*this);
  }
}

void WalkVisitor::Visit(VarDeclStmt& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void WalkVisitor::Visit(ExprStmt& node) {
  if (auto* e = node.MutableExpression()) {
    e->Accept(*this);
  }
}

void WalkVisitor::Visit(ReturnStmt& node) {
  if (auto* v = node.MutableValue()) {
    v->Accept(*this);
  }
}

void WalkVisitor::Visit(BreakStmt& node) {
  (void) node;
}

void WalkVisitor::Visit(ContinueStmt& node) {
  (void) node;
}

void WalkVisitor::Visit(IfStmt& node) {
  for (auto& br : node.MutableBranches()) {
    if (auto* c = br.MutableCondition()) {
      c->Accept(*this);
    }

    if (auto* t = br.MutableThen()) {
      t->Accept(*this);
    }
  }

  if (auto* eb = node.MutableElseBlock()) {
    eb->Accept(*this);
  }
}

void WalkVisitor::Visit(WhileStmt& node) {
  if (auto* c = node.MutableCondition()) {
    c->Accept(*this);
  }

  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void WalkVisitor::Visit(ForStmt& node) {
  if (auto* it = node.MutableIteratorExpr()) {
    it->Accept(*this);
  }

  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void WalkVisitor::Visit(UnsafeBlock& node) {
  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void WalkVisitor::Visit(Binary& node) {
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
}

void WalkVisitor::Visit(Unary& node) {
  node.MutableOperand().Accept(*this);
}

void WalkVisitor::Visit(Assign& node) {
  node.MutableTarget().Accept(*this);
  node.MutableValue().Accept(*this);
}

void WalkVisitor::Visit(Call& node) {
  node.MutableCallee().Accept(*this);
  for (auto& a : node.MutableArgs()) {
    a->Accept(*this);
  }
}

void WalkVisitor::Visit(FieldAccess& node) {
  node.MutableObject().Accept(*this);
}

void WalkVisitor::Visit(IndexAccess& node) {
  node.MutableObject().Accept(*this);
  node.MutableIndexExpr().Accept(*this);
}

void WalkVisitor::Visit(NamespaceRef& node) {
  node.MutableNamespaceExpr().Accept(*this);
}

void WalkVisitor::Visit(SafeCall& node) {
  node.MutableObject().Accept(*this);
  for (auto& a : node.MutableArgs()) {
    a->Accept(*this);
  }
}

void WalkVisitor::Visit(Elvis& node) {
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
}

void WalkVisitor::Visit(CastAs& node) {
  node.MutableExpression().Accept(*this);
}

void WalkVisitor::Visit(TypeTestIs& node) {
  node.MutableExpression().Accept(*this);
}

void WalkVisitor::Visit(IdentRef& node) {
  (void) node;
}

void WalkVisitor::Visit(IntLit& node) {
  (void) node;
}

void WalkVisitor::Visit(FloatLit& node) {
  (void) node;
}

void WalkVisitor::Visit(StringLit& node) {
  (void) node;
}

void WalkVisitor::Visit(CharLit& node) {
  (void) node;
}

void WalkVisitor::Visit(BoolLit& node) {
  (void) node;
}

void WalkVisitor::Visit(NullLit& node) {
  (void) node;
}
void WalkVisitor::Visit(ThisExpr& node) {
  (void) node;
}

} // namespace ovum::compiler::parser
