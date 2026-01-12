#include "StructuralValidator.hpp"

#include "lib/parser/diagnostics/severity/Severity.hpp"

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"

#include "lib/parser/ast/nodes/exprs/Binary.hpp"
#include "lib/parser/ast/nodes/exprs/Call.hpp"
#include "lib/parser/ast/nodes/exprs/FieldAccess.hpp"
#include "lib/parser/ast/nodes/exprs/IndexAccess.hpp"
#include "lib/parser/ast/nodes/exprs/NamespaceRef.hpp"
#include "lib/parser/ast/nodes/exprs/SafeCall.hpp"
#include "lib/parser/ast/nodes/exprs/Unary.hpp"

namespace ovum::compiler::parser {

void StructuralValidator::Visit(Module& node) {
  if (node.Name().empty()) {
    sink_.Error("E0001", "module name must not be empty");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(FunctionDecl& node) {
  if (!node.IsPure() && node.MutableBody() == nullptr) {
    sink_.Error("E1001", "function must have a body");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(ClassDecl& node) {
  if (node.Name().empty()) {
    sink_.Error("E1002", "class name must not be empty");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(CallDecl& node) {
  if (node.MutableBody() == nullptr) {
    sink_.Error("E1101", "call declaration must have a body");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(MethodDecl& node) {
  if (!node.IsPure() && node.MutableBody() == nullptr) {
    sink_.Error("E1201", "method must have a body");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(DestructorDecl& node) {
  if (node.MutableBody() == nullptr) {
    sink_.Error("E1301", "destructor must have a body");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(Call& node) {
  if (&node.MutableCallee() == nullptr) {
    sink_.Error("E2001", "call must have callee");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(Binary& node) {
  (void) node.MutableLhs();
  (void) node.MutableRhs();
  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(Unary& node) {
  (void) node.MutableOperand();
  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(FieldAccess& node) {
  (void) node.MutableObject();
  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(IndexAccess& node) {
  (void) node.MutableObject();
  (void) node.MutableIndexExpr();
  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(NamespaceRef& node) {
  if (node.Name().empty()) {
    sink_.Error("E2101", "namespace reference must have name");
  }

  WalkVisitor::Visit(node);
}

void StructuralValidator::Visit(SafeCall& node) {
  if (node.Method().empty()) {
    sink_.Error("E2201", "safecall must have method name");
  }

  (void) node.MutableObject();
  WalkVisitor::Visit(node);
}

} // namespace ovum::compiler::parser
