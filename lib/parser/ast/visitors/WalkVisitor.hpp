#ifndef PARSER_WALKVISITOR_HPP_
#define PARSER_WALKVISITOR_HPP_

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

class WalkVisitor : public AstVisitor {
public:
  ~WalkVisitor() override = default;

  void Visit(Module& node) override;
  void Visit(FunctionDecl& node) override;
  void Visit(ClassDecl& node) override;
  void Visit(InterfaceMethod& node) override;
  void Visit(InterfaceDecl& node) override;
  void Visit(TypeAliasDecl& node) override;
  void Visit(GlobalVarDecl& node) override;
  void Visit(FieldDecl& node) override;
  void Visit(StaticFieldDecl& node) override;
  void Visit(MethodDecl& node) override;
  void Visit(CallDecl& node) override;
  void Visit(DestructorDecl& node) override;

  void Visit(Block& node) override;
  void Visit(VarDeclStmt& node) override;
  void Visit(ExprStmt& node) override;
  void Visit(ReturnStmt& node) override;
  void Visit(BreakStmt& node) override;
  void Visit(ContinueStmt& node) override;
  void Visit(IfStmt& node) override;
  void Visit(WhileStmt& node) override;
  void Visit(ForStmt& node) override;
  void Visit(UnsafeBlock& node) override;

  void Visit(Binary& node) override;
  void Visit(Unary& node) override;
  void Visit(Assign& node) override;
  void Visit(Call& node) override;
  void Visit(FieldAccess& node) override;
  void Visit(IndexAccess& node) override;
  void Visit(NamespaceRef& node) override;
  void Visit(SafeCall& node) override;
  void Visit(Elvis& node) override;
  void Visit(CastAs& node) override;
  void Visit(TypeTestIs& node) override;
  void Visit(IdentRef& node) override;
  void Visit(IntLit& node) override;
  void Visit(FloatLit& node) override;
  void Visit(StringLit& node) override;
  void Visit(CharLit& node) override;
  void Visit(BoolLit& node) override;
  void Visit(NullLit& node) override;
  void Visit(ThisExpr& node) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_WALKVISITOR_HPP_
