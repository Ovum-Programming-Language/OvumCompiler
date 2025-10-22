#ifndef ASTVISITOR_HPP_
#define ASTVISITOR_HPP_

class Module;
class FunctionDecl;
class ClassDecl;
class InterfaceDecl;
class TypeAliasDecl;
class GlobalVarDecl;
class FieldDecl;
class StaticFieldDecl;
class MethodDecl;
class CallDecl;
class DestructorDecl;

class Block;
class VarDeclStmt;
class ExprStmt;
class ReturnStmt;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class UnsafeBlock;

class Binary;
class Unary;
class Assign;
class Call;
class FieldAccess;
class IndexAccess;
class NamespaceRef;
class SafeCall;
class Elvis;
class CastAs;
class TypeTestIs;
class IdentRef;
class IntLit;
class FloatLit;
class StringLit;
class CharLit;
class BoolLit;
class NullLit;

class AstVisitor {
public:
  virtual ~AstVisitor() = default;

  // Decls
  virtual void Visit(Module&) = 0;
  virtual void Visit(FunctionDecl&) = 0;
  virtual void Visit(ClassDecl&) = 0;
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

#endif // ASTVISITOR_HPP_
