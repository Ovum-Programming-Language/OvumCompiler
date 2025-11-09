#include "ConstWalkVisitor.hpp"

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

void ConstWalkVisitor::Visit(Module& node) {
  for (auto& decl_ptr : node.MutableDecls()) {
    decl_ptr->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(FunctionDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(ClassDecl& node) {
  for (auto& member : node.MutableMembers()) {
    member->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(InterfaceMethod& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(InterfaceDecl& node) {
  for (auto& m : node.MutableMembers()) {
    m->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(TypeAliasDecl& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(GlobalVarDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(FieldDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(StaticFieldDecl& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(MethodDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(CallDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(DestructorDecl& node) {
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(Block& node) {
  for (auto& stmt : node.GetStatements()) {
    stmt->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(VarDeclStmt& node) {
  if (auto* init = node.MutableInit()) {
    init->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(ExprStmt& node) {
  if (auto* e = node.MutableExpression()) {
    e->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(ReturnStmt& node) {
  if (auto* v = node.MutableValue()) {
    v->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(BreakStmt& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(ContinueStmt& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(IfStmt& node) {
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

void ConstWalkVisitor::Visit(WhileStmt& node) {
  if (auto* c = node.MutableCondition()) {
    c->Accept(*this);
  }

  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(ForStmt& node) {
  if (auto* it = node.MutableIteratorExpr()) {
    it->Accept(*this);
  }

  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(UnsafeBlock& node) {
  if (auto* b = node.MutableBody()) {
    b->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(Binary& node) {
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
}

void ConstWalkVisitor::Visit(Unary& node) {
  node.MutableOperand().Accept(*this);
}

void ConstWalkVisitor::Visit(Assign& node) {
  node.MutableTarget().Accept(*this);
  node.MutableValue().Accept(*this);
}

void ConstWalkVisitor::Visit(Call& node) {
  node.MutableCallee().Accept(*this);
  for (auto& a : node.MutableArgs()) {
    a->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(FieldAccess& node) {
  node.MutableObject().Accept(*this);
}

void ConstWalkVisitor::Visit(IndexAccess& node) {
  node.MutableObject().Accept(*this);
  node.MutableIndexExpr().Accept(*this);
}

void ConstWalkVisitor::Visit(NamespaceRef& node) {
  node.MutableNamespaceExpr().Accept(*this);
}

void ConstWalkVisitor::Visit(SafeCall& node) {
  node.MutableObject().Accept(*this);
  for (auto& a : node.MutableArgs()) {
    a->Accept(*this);
  }
}

void ConstWalkVisitor::Visit(Elvis& node) {
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
}

void ConstWalkVisitor::Visit(CastAs& node) {
  node.MutableExpression().Accept(*this);
}

void ConstWalkVisitor::Visit(TypeTestIs& node) {
  node.MutableExpression().Accept(*this);
}

void ConstWalkVisitor::Visit(IdentRef& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(IntLit& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(FloatLit& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(StringLit& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(CharLit& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(BoolLit& node) {
  (void) node;
}

void ConstWalkVisitor::Visit(NullLit& node) {
  (void) node;
}

} // namespace ovum::compiler::parser
