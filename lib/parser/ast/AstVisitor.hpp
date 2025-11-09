#ifndef PARSER_ASTVISITOR_HPP_
#define PARSER_ASTVISITOR_HPP_

#include "nodes/class_members/CallDecl.hpp"
#include "nodes/class_members/DestructorDecl.hpp"
#include "nodes/class_members/FieldDecl.hpp"
#include "nodes/class_members/MethodDecl.hpp"
#include "nodes/class_members/StaticFieldDecl.hpp"
#include "nodes/decls/ClassDecl.hpp"
#include "nodes/decls/FunctionDecl.hpp"
#include "nodes/decls/GlobalVarDecl.hpp"
#include "nodes/decls/InterfaceDecl.hpp"
#include "nodes/decls/Module.hpp"
#include "nodes/decls/TypeAliasDecl.hpp"
#include "nodes/exprs/Assign.hpp"
#include "nodes/exprs/Binary.hpp"
#include "nodes/exprs/Call.hpp"
#include "nodes/exprs/CastAs.hpp"
#include "nodes/exprs/Elvis.hpp"
#include "nodes/exprs/FieldAccess.hpp"
#include "nodes/exprs/IdentRef.hpp"
#include "nodes/exprs/IndexAccess.hpp"
#include "nodes/exprs/NamespaceRef.hpp"
#include "nodes/exprs/SafeCall.hpp"
#include "nodes/exprs/TypeTestIs.hpp"
#include "nodes/exprs/Unary.hpp"
#include "nodes/exprs/literals/BoolLit.hpp"
#include "nodes/exprs/literals/CharLit.hpp"
#include "nodes/exprs/literals/FloatLit.hpp"
#include "nodes/exprs/literals/IntLit.hpp"
#include "nodes/exprs/literals/NullLit.hpp"
#include "nodes/exprs/literals/StringLit.hpp"
#include "nodes/stmts/BreakStmt.hpp"
#include "nodes/stmts/ContinueStmt.hpp"
#include "nodes/stmts/ExprStmt.hpp"
#include "nodes/stmts/ForStmt.hpp"
#include "nodes/stmts/IfStmt.hpp"
#include "nodes/stmts/ReturnStmt.hpp"
#include "nodes/stmts/UnsafeBlock.hpp"
#include "nodes/stmts/VarDeclStmt.hpp"
#include "nodes/stmts/WhileStmt.hpp"

namespace ovum::compiler::parser {

class AstVisitor {
public:
  virtual ~AstVisitor() = default;

  // Decls
  virtual void Visit(Module&) = 0;
  virtual void Visit(FunctionDecl&) = 0;
  virtual void Visit(ClassDecl&) = 0;
  virtual void Visit(InterfaceMethod&) = 0;
  virtual void Visit(InterfaceDecl&) = 0;
  virtual void Visit(TypeAliasDecl&) = 0;
  virtual void Visit(GlobalVarDecl&) = 0;
  virtual void Visit(FieldDecl&) = 0;
  virtual void Visit(StaticFieldDecl&) = 0;
  virtual void Visit(MethodDecl&) = 0;
  virtual void Visit(CallDecl&) = 0;
  virtual void Visit(DestructorDecl&) = 0;

  // Stmts
  virtual void Visit(Block&) = 0;
  virtual void Visit(VarDeclStmt&) = 0;
  virtual void Visit(ExprStmt&) = 0;
  virtual void Visit(ReturnStmt&) = 0;
  virtual void Visit(BreakStmt&) = 0;
  virtual void Visit(ContinueStmt&) = 0;
  virtual void Visit(IfStmt&) = 0;
  virtual void Visit(WhileStmt&) = 0;
  virtual void Visit(ForStmt&) = 0;
  virtual void Visit(UnsafeBlock&) = 0;

  // Exprs
  virtual void Visit(Binary&) = 0;
  virtual void Visit(Unary&) = 0;
  virtual void Visit(Assign&) = 0;
  virtual void Visit(Call&) = 0;
  virtual void Visit(FieldAccess&) = 0;
  virtual void Visit(IndexAccess&) = 0;
  virtual void Visit(NamespaceRef&) = 0;
  virtual void Visit(SafeCall&) = 0;
  virtual void Visit(Elvis&) = 0;
  virtual void Visit(CastAs&) = 0;
  virtual void Visit(TypeTestIs&) = 0;
  virtual void Visit(IdentRef&) = 0;
  virtual void Visit(IntLit&) = 0;
  virtual void Visit(FloatLit&) = 0;
  virtual void Visit(StringLit&) = 0;
  virtual void Visit(CharLit&) = 0;
  virtual void Visit(BoolLit&) = 0;
  virtual void Visit(NullLit&) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ASTVISITOR_HPP_
