#ifndef PARSER_IASTFACTORY_HPP_
#define PARSER_IASTFACTORY_HPP_

#include <memory>
#include <optional>
#include <string>
#include <vector>

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
#include "nodes/stmts/Block.hpp"
#include "nodes/stmts/BreakStmt.hpp"
#include "nodes/stmts/ContinueStmt.hpp"
#include "nodes/stmts/ExprStmt.hpp"
#include "nodes/stmts/ForStmt.hpp"
#include "nodes/stmts/IfStmt.hpp"
#include "nodes/stmts/ReturnStmt.hpp"
#include "nodes/stmts/UnsafeBlock.hpp"
#include "nodes/stmts/VarDeclStmt.hpp"
#include "nodes/stmts/WhileStmt.hpp"

#include "lib/parser/ast/nodes/exprs/tags/IAssignOpTag.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IBinaryOpTag.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IUnaryOpTag.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"
#include "nodes/exprs/ThisExpr.hpp"

namespace ovum::compiler::parser {

class IAstFactory {
public:
  virtual ~IAstFactory() = default;

  // Module / Decls
  virtual std::unique_ptr<Module> MakeModule(std::string name,
                                             SourceId source_id,
                                             std::vector<std::unique_ptr<Decl>> decls,
                                             SourceSpan span) = 0;

  virtual std::unique_ptr<FunctionDecl> MakeFunction(bool is_pure,
                                                     std::string name,
                                                     std::vector<Param> params,
                                                     std::unique_ptr<TypeReference> return_type,
                                                     std::unique_ptr<Block> body,
                                                     SourceSpan span) = 0;

  virtual std::unique_ptr<ClassDecl> MakeClass(std::string name,
                                               std::vector<TypeReference> implements,
                                               std::vector<std::unique_ptr<Decl>> members,
                                               SourceSpan span) = 0;

  virtual std::unique_ptr<InterfaceDecl> MakeInterface(std::string name,
                                                       std::vector<std::unique_ptr<InterfaceMethod>> methods,
                                                       SourceSpan span) = 0;

  virtual std::unique_ptr<InterfaceMethod> MakeInterfaceMethod(std::string name,
                                                               std::vector<Param> params,
                                                               std::unique_ptr<TypeReference> return_type,
                                                               SourceSpan span) = 0;

  virtual std::unique_ptr<TypeAliasDecl> MakeTypeAlias(std::string name,
                                                       TypeReference aliased_type,
                                                       SourceSpan span) = 0;

  virtual std::unique_ptr<GlobalVarDecl> MakeGlobalVar(
      bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) = 0;

  // Class members
  virtual std::unique_ptr<FieldDecl> MakeField(bool is_public,
                                               bool is_var,
                                               std::string name,
                                               TypeReference type,
                                               std::unique_ptr<Expr> init,
                                               SourceSpan span) = 0;

  virtual std::unique_ptr<StaticFieldDecl> MakeStaticField(bool is_public,
                                                           bool is_var,
                                                           std::string name,
                                                           TypeReference type,
                                                           std::unique_ptr<Expr> init,
                                                           SourceSpan span) = 0;

  virtual std::unique_ptr<MethodDecl> MakeMethod(bool is_public,
                                                 bool is_override,
                                                 bool is_static,
                                                 bool is_pure,
                                                 std::string name,
                                                 std::vector<Param> params,
                                                 std::unique_ptr<TypeReference> ret_type,
                                                 std::unique_ptr<Block> body,
                                                 SourceSpan span) = 0;

  virtual std::unique_ptr<CallDecl> MakeCallDecl(bool is_public,
                                                 std::vector<Param> params,
                                                 std::unique_ptr<TypeReference> ret_type,
                                                 std::unique_ptr<Block> body,
                                                 SourceSpan span) = 0;

  virtual std::unique_ptr<DestructorDecl> MakeDestructor(bool is_public,
                                                         std::unique_ptr<Block> body,
                                                         SourceSpan span) = 0;

  // Statements
  virtual std::unique_ptr<Block> MakeBlock(std::vector<std::unique_ptr<Stmt>> stmts, SourceSpan span) = 0;

  virtual std::unique_ptr<VarDeclStmt> MakeVarDeclStmt(
      bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) = 0;

  virtual std::unique_ptr<ExprStmt> MakeExprStmt(std::unique_ptr<Expr> expr, SourceSpan span) = 0;

  virtual std::unique_ptr<ReturnStmt> MakeReturnStmt(std::unique_ptr<Expr> value, SourceSpan span) = 0;

  virtual std::unique_ptr<BreakStmt> MakeBreakStmt(SourceSpan span) = 0;
  virtual std::unique_ptr<ContinueStmt> MakeContinueStmt(SourceSpan span) = 0;

  virtual std::unique_ptr<IfStmt> MakeIfStmt(std::vector<Branch> branches,
                                             std::unique_ptr<Block> else_block,
                                             SourceSpan span) = 0;

  virtual std::unique_ptr<WhileStmt> MakeWhileStmt(std::unique_ptr<Expr> cond,
                                                   std::unique_ptr<Block> body,
                                                   SourceSpan span) = 0;

  virtual std::unique_ptr<ForStmt> MakeForStmt(std::string iter_name,
                                               std::unique_ptr<Expr> iter_expr,
                                               std::unique_ptr<Block> body,
                                               SourceSpan span) = 0;

  virtual std::unique_ptr<UnsafeBlock> MakeUnsafeBlock(std::unique_ptr<Block> body, SourceSpan span) = 0;

  // Expressions
  virtual std::unique_ptr<Binary> MakeBinary(const IBinaryOpTag& op,
                                             std::unique_ptr<Expr> lhs,
                                             std::unique_ptr<Expr> rhs,
                                             SourceSpan span) = 0;

  virtual std::unique_ptr<Unary> MakeUnary(const IUnaryOpTag& op, std::unique_ptr<Expr> operand, SourceSpan span) = 0;

  virtual std::unique_ptr<Assign> MakeAssign(const IAssignOpTag& op,
                                             std::unique_ptr<Expr> target,
                                             std::unique_ptr<Expr> value,
                                             SourceSpan span) = 0;

  virtual std::unique_ptr<Call> MakeCall(std::unique_ptr<Expr> callee,
                                         std::vector<std::unique_ptr<Expr>> args,
                                         SourceSpan span) = 0;

  virtual std::unique_ptr<FieldAccess> MakeFieldAccess(std::unique_ptr<Expr> object,
                                                       std::string name,
                                                       SourceSpan span) = 0;

  virtual std::unique_ptr<IndexAccess> MakeIndexAccess(std::unique_ptr<Expr> object,
                                                       std::unique_ptr<Expr> index,
                                                       SourceSpan span) = 0;

  virtual std::unique_ptr<NamespaceRef> MakeNamespaceRef(std::unique_ptr<Expr> ns,
                                                         std::string name,
                                                         SourceSpan span) = 0;

  virtual std::unique_ptr<SafeCall> MakeSafeCall(std::unique_ptr<Expr> object,
                                                 std::string method,
                                                 std::vector<std::unique_ptr<Expr>> args,
                                                 std::optional<TypeReference> inferred_type,
                                                 SourceSpan span) = 0;

  virtual std::unique_ptr<Elvis> MakeElvis(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs, SourceSpan span) = 0;

  virtual std::unique_ptr<CastAs> MakeCastAs(std::unique_ptr<Expr> expr, TypeReference type, SourceSpan span) = 0;

  virtual std::unique_ptr<TypeTestIs> MakeTypeTestIs(std::unique_ptr<Expr> expr,
                                                     TypeReference type,
                                                     SourceSpan span) = 0;

  virtual std::unique_ptr<IdentRef> MakeIdent(std::string name, SourceSpan span) = 0;
  virtual std::unique_ptr<IntLit> MakeInt(long long v, SourceSpan span) = 0;
  virtual std::unique_ptr<FloatLit> MakeFloat(long double v, SourceSpan span) = 0;
  virtual std::unique_ptr<StringLit> MakeString(std::string v, SourceSpan span) = 0;
  virtual std::unique_ptr<CharLit> MakeChar(char v, SourceSpan span) = 0;
  virtual std::unique_ptr<BoolLit> MakeBool(bool v, SourceSpan span) = 0;
  virtual std::unique_ptr<NullLit> MakeNull(SourceSpan span) = 0;
  virtual std::unique_ptr<ThisExpr> MakeThisExpr(SourceSpan span) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IASTFACTORY_HPP_
