#ifndef IASTFACTORY_HPP_
#define IASTFACTORY_HPP_

#include <memory>

#include "nodes/class_members/CallDecl.hpp"
#include "nodes/class_members/DestructorDecl.hpp"
#include "nodes/class_members/FieldDecl.hpp"
#include "nodes/class_members/MethodDecl.hpp"
#include "nodes/class_members/StaticFieldDecl.hpp"
#include "nodes/decls/ClassDecl.hpp"
#include "nodes/decls/FunctionDecl.hpp"
#include "nodes/decls/GlobalVarDecl.hpp"
#include "nodes/decls/InterfaceDecl.hpp"
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

class IAstFactory {
public:
  virtual ~IAstFactory() = default;

  virtual std::unique_ptr<FunctionDecl> MakeFunction() = 0;
  virtual std::unique_ptr<ClassDecl> MakeClass() = 0;
  virtual std::unique_ptr<InterfaceDecl> MakeInterface() = 0;
  virtual std::unique_ptr<InterfaceMethod> MakeInterfaceMethod() = 0;
  virtual std::unique_ptr<TypeAliasDecl> MakeTypeAlias() = 0;
  virtual std::unique_ptr<GlobalVarDecl> MakeGlobalVar() = 0;

  virtual std::unique_ptr<FieldDecl> MakeField() = 0;
  virtual std::unique_ptr<StaticFieldDecl> MakeStaticField() = 0;
  virtual std::unique_ptr<MethodDecl> MakeMethod() = 0;
  virtual std::unique_ptr<CallDecl> MakeCallDecl() = 0;
  virtual std::unique_ptr<DestructorDecl> MakeDestructor() = 0;

  virtual std::unique_ptr<Block> MakeBlock() = 0;
  virtual std::unique_ptr<VarDeclStmt> MakeVarDeclStmt() = 0;
  virtual std::unique_ptr<ExprStmt> MakeExprStmt() = 0;
  virtual std::unique_ptr<ReturnStmt> MakeReturnStmt() = 0;
  virtual std::unique_ptr<BreakStmt> MakeBreakStmt() = 0;
  virtual std::unique_ptr<ContinueStmt> MakeContinueStmt() = 0;
  virtual std::unique_ptr<IfStmt> MakeIfStmt() = 0;
  virtual std::unique_ptr<WhileStmt> MakeWhileStmt() = 0;
  virtual std::unique_ptr<ForStmt> MakeForStmt() = 0;
  virtual std::unique_ptr<UnsafeBlock> MakeUnsafeBlock() = 0;

  virtual std::unique_ptr<Binary> MakeBinary(const IBinaryOpTag& op) = 0;
  virtual std::unique_ptr<Unary> MakeUnary(const IUnaryOpTag& op) = 0;
  virtual std::unique_ptr<Assign> MakeAssign(const IAssignOpTag& op) = 0;
  virtual std::unique_ptr<Call> MakeCall() = 0;
  virtual std::unique_ptr<FieldAccess> MakeFieldAccess() = 0;
  virtual std::unique_ptr<IndexAccess> MakeIndexAccess() = 0;
  virtual std::unique_ptr<NamespaceRef> MakeNamespaceRef() = 0;
  virtual std::unique_ptr<SafeCall> MakeSafeCall() = 0;
  virtual std::unique_ptr<Elvis> MakeElvis() = 0;
  virtual std::unique_ptr<CastAs> MakeCastAs() = 0;
  virtual std::unique_ptr<TypeTestIs> MakeTypeTestIs() = 0;
  virtual std::unique_ptr<IdentRef> MakeIdent(std::string name) = 0;
  virtual std::unique_ptr<IntLit> MakeInt(long long v) = 0;
  virtual std::unique_ptr<FloatLit> MakeFloat(long double v) = 0;
  virtual std::unique_ptr<StringLit> MakeString(std::u32string v) = 0;
  virtual std::unique_ptr<CharLit> MakeChar(char32_t v) = 0;
  virtual std::unique_ptr<BoolLit> MakeBool(bool v) = 0;
  virtual std::unique_ptr<NullLit> MakeNull() = 0;
};

#endif // IASTFACTORY_HPP_
