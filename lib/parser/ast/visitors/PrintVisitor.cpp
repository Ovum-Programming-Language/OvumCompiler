#include "PrintVisitor.hpp"

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"

#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"

#include "lib/parser/ast/nodes/stmts/Block.hpp"

namespace ovum::compiler::parser {

PrintVisitor::PrintVisitor(std::ostream& output) : out_(&output) {
}

PrintVisitor::PrintVisitor() : out_(&buffer_), use_buffer_(true) {
}

std::string PrintVisitor::Str() const {
  return buffer_.str();
}

void PrintVisitor::WriteIndent() {
  for (int i = 0; i < indent_; ++i) {
    *out_ << "  ";
  }
}

void PrintVisitor::WriteLine(const std::string& text) {
  WriteIndent();
  *out_ << text << '\n';
}

void PrintVisitor::Open(const std::string& header) {
  WriteLine(header + " {");
  ++indent_;
}

void PrintVisitor::Close() {
  --indent_;
  WriteLine("}");
}

void PrintVisitor::Visit(Module& node) {
  Open("Module name=\"" + node.Name() + "\"");
  for (auto& declaration : node.MutableDecls()) {
    declaration->Accept(*this);
  }
  Close();
}

void PrintVisitor::Visit(FunctionDecl& node) {
  Open(std::string("FunctionDecl name=\"") + node.Name() + "\"" + (node.IsPure() ? " pure" : ""));
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }
  Close();
}

void PrintVisitor::Visit(ClassDecl& node) {
  Open(std::string("ClassDecl name=\"") + node.Name() + "\"");
  for (auto& member : node.MutableMembers()) {
    member->Accept(*this);
  }
  Close();
}

void PrintVisitor::Visit(InterfaceMethod& node) {
  WriteLine(std::string("InterfaceMethod name=\"") + node.Name() + "\"");
}

void PrintVisitor::Visit(InterfaceDecl& node) {
  Open(std::string("InterfaceDecl name=\"") + node.Name() + "\"");
  for (auto& method_ptr : node.MutableMembers()) {
    method_ptr->Accept(*this);
  }
  Close();
}

void PrintVisitor::Visit(TypeAliasDecl& node) {
  WriteLine(std::string("TypeAliasDecl name=\"") + node.Name() + "\"");
}

void PrintVisitor::Visit(GlobalVarDecl& node) {
  Open(std::string("GlobalVarDecl ") + (node.IsVar() ? "var" : "val") + " name=\"" + node.Name() + "\"");
  if (auto* init_expr = node.MutableInit()) {
    init_expr->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(FieldDecl& node) {
  Open(std::string("FieldDecl ") + (node.IsVar() ? "var" : "val") + " name=\"" + node.Name() + "\"" +
       (node.IsPublic() ? " public" : ""));
  if (auto* init_expr = node.MutableInit()) {
    init_expr->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(StaticFieldDecl& node) {
  Open(std::string("StaticFieldDecl ") + (node.IsVar() ? "var" : "val") + " name=\"" + node.Name() + "\"" +
       (node.IsPublic() ? " public" : ""));
  if (auto* init_expr = node.MutableInit()) {
    init_expr->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(MethodDecl& node) {
  std::string flags;
  if (node.IsPublic()) {
    flags += " public";
  }

  if (node.IsStatic()) {
    flags += " static";
  }

  if (node.IsOverride()) {
    flags += " override";
  }

  if (node.IsPure()) {
    flags += " pure";
  }

  Open("MethodDecl name=\"" + node.Name() + "\"" + flags);
  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(CallDecl& node) {
  Open(std::string("CallDecl") + (node.IsPublic() ? " public" : ""));

  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(DestructorDecl& node) {
  Open(std::string("DestructorDecl") + (node.IsPublic() ? " public" : ""));

  if (auto* body = node.MutableBody()) {
    body->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(Block& node) {
  Open("Block");

  for (auto& statement : node.GetStatements()) {
    statement->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(VarDeclStmt& node) {
  Open(std::string("VarDeclStmt ") + (node.IsVar() ? "var" : "val") + " name=\"" + node.Name() + "\"");

  if (auto* init_expr = node.MutableInit()) {
    init_expr->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(ExprStmt& node) {
  Open("ExprStmt");

  if (auto* expression = node.MutableExpression()) {
    expression->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(ReturnStmt& node) {
  if (!node.HasValue()) {
    WriteLine("ReturnStmt");
    return;
  }

  Open("ReturnStmt");
  if (auto* value_expr = node.MutableValue()) {
    value_expr->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(BreakStmt& node) {
  WriteLine("BreakStmt");
  (void) node;
}

void PrintVisitor::Visit(ContinueStmt& node) {
  WriteLine("ContinueStmt");
  (void) node;
}

void PrintVisitor::Visit(IfStmt& node) {
  Open("IfStmt");
  for (auto& branch_value : node.MutableBranches()) {
    Open("Branch");

    if (auto* cond_expr = branch_value.MutableCondition()) {
      Open("Condition");
      cond_expr->Accept(*this);
      Close();
    }

    if (auto* then_block = branch_value.MutableThen()) {
      Open("Then");
      then_block->Accept(*this);
      Close();
    }

    Close();
  }

  if (auto* else_block = node.MutableElseBlock()) {
    Open("Else");
    else_block->Accept(*this);
    Close();
  }

  Close();
}

void PrintVisitor::Visit(WhileStmt& node) {
  Open("WhileStmt");

  if (auto* cond_expr = node.MutableCondition()) {
    Open("Condition");
    cond_expr->Accept(*this);
    Close();
  }

  if (auto* body_block = node.MutableBody()) {
    Open("Body");
    body_block->Accept(*this);
    Close();
  }

  Close();
}

void PrintVisitor::Visit(ForStmt& node) {
  Open(std::string("ForStmt iterator=\"") + node.IteratorName() + "\"");

  if (auto* iter_expr = node.MutableIteratorExpr()) {
    Open("Iterable");
    iter_expr->Accept(*this);
    Close();
  }

  if (auto* body_block = node.MutableBody()) {
    Open("Body");
    body_block->Accept(*this);
    Close();
  }

  Close();
}

void PrintVisitor::Visit(UnsafeBlock& node) {
  Open("UnsafeBlock");

  if (auto* body_block = node.MutableBody()) {
    body_block->Accept(*this);
  }

  Close();
}

void PrintVisitor::Visit(Binary& node) {
  Open(std::string("Binary op=\"") + std::string(node.Op().Name()) + "\"");
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
  Close();
}

void PrintVisitor::Visit(Unary& node) {
  Open(std::string("Unary op=\"") + std::string(node.Op().Name()) + "\"");
  node.MutableOperand().Accept(*this);
  Close();
}

void PrintVisitor::Visit(Assign& node) {
  Open(std::string("Assign kind=\"") + std::string(node.Kind().Name()) + "\"");
  Open("Target");
  node.MutableTarget().Accept(*this);
  Close();
  Open("Value");
  node.MutableValue().Accept(*this);
  Close();
  Close();
}

void PrintVisitor::Visit(Call& node) {
  Open("Call");
  Open("Callee");
  node.MutableCallee().Accept(*this);
  Close();
  Open("Args");

  for (auto& argument : node.MutableArgs()) {
    argument->Accept(*this);
  }

  Close();
  Close();
}

void PrintVisitor::Visit(FieldAccess& node) {
  Open(std::string("FieldAccess name=\"") + node.Name() + "\"");
  node.MutableObject().Accept(*this);
  Close();
}

void PrintVisitor::Visit(IndexAccess& node) {
  Open("IndexAccess");
  Open("Object");
  node.MutableObject().Accept(*this);
  Close();
  Open("Index");
  node.MutableIndexExpr().Accept(*this);
  Close();
  Close();
}

void PrintVisitor::Visit(NamespaceRef& node) {
  Open(std::string("NamespaceRef name=\"") + node.Name() + "\"");
  node.MutableNamespaceExpr().Accept(*this);
  Close();
}

void PrintVisitor::Visit(SafeCall& node) {
  Open(std::string("SafeCall method=\"") + node.Method() + "\"");
  Open("Object");
  node.MutableObject().Accept(*this);
  Close();
  Open("Args");

  for (auto& argument : node.MutableArgs()) {
    argument->Accept(*this);
  }

  Close();
  Close();
}

void PrintVisitor::Visit(Elvis& node) {
  Open("Elvis");
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
  Close();
}

void PrintVisitor::Visit(CastAs& node) {
  Open("CastAs");
  node.MutableExpression().Accept(*this);
  Close();
}

void PrintVisitor::Visit(TypeTestIs& node) {
  Open("TypeTestIs");
  node.MutableExpression().Accept(*this);
  Close();
}

void PrintVisitor::Visit(IdentRef& node) {
  WriteLine(std::string("IdentRef name=\"") + node.Name() + "\"");
}

void PrintVisitor::Visit(IntLit& node) {
  WriteLine("IntLit value=" + std::to_string(node.Value()));
}

void PrintVisitor::Visit(FloatLit& node) {
  WriteLine("FloatLit value=" + std::to_string(node.Value()));
}

void PrintVisitor::Visit(StringLit& node) {
  WriteLine(std::string("StringLit value=\"") + node.Value() + "\"");
}

void PrintVisitor::Visit(CharLit& node) {
  std::string printable(1, node.Value());
  WriteLine(std::string("CharLit value='") + printable + "'");
}

void PrintVisitor::Visit(BoolLit& node) {
  WriteLine(std::string("BoolLit value=") + (node.Value() ? "true" : "false"));
}

void PrintVisitor::Visit(NullLit& node) {
  WriteLine("NullLit");
}

} // namespace ovum::compiler::parser
