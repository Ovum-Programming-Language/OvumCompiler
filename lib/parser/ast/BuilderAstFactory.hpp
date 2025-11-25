#ifndef PARSER_BUILDERASTFACTORY_HPP_
#define PARSER_BUILDERASTFACTORY_HPP_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "IAstFactory.hpp"

namespace ovum::compiler::parser {

class BuilderAstFactory : public IAstFactory {
public:
  ~BuilderAstFactory() override = default;

  // Module / Decls
  std::unique_ptr<Module> MakeModule(std::string name,
                                     SourceId source_id,
                                     std::vector<std::unique_ptr<Decl>> decls,
                                     SourceSpan span) override;

  std::unique_ptr<FunctionDecl> MakeFunction(bool is_pure,
                                             std::string name,
                                             std::vector<Param> params,
                                             std::unique_ptr<TypeReference> return_type,
                                             std::unique_ptr<Block> body,
                                             SourceSpan span) override;

  std::unique_ptr<ClassDecl> MakeClass(std::string name,
                                       std::vector<TypeReference> implements,
                                       std::vector<std::unique_ptr<Decl>> members,
                                       SourceSpan span) override;

  std::unique_ptr<InterfaceDecl> MakeInterface(std::string name,
                                               std::vector<std::unique_ptr<InterfaceMethod>> methods,
                                               SourceSpan span) override;

  std::unique_ptr<InterfaceMethod> MakeInterfaceMethod(std::string name,
                                                       std::vector<InterfaceMethod::Param> params,
                                                       std::unique_ptr<TypeReference> return_type,
                                                       SourceSpan span) override;

  std::unique_ptr<TypeAliasDecl> MakeTypeAlias(std::string name, TypeReference aliased_type, SourceSpan span) override;

  std::unique_ptr<GlobalVarDecl> MakeGlobalVar(
      bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) override;

  // Class members
  std::unique_ptr<FieldDecl> MakeField(bool is_public,
                                       bool is_var,
                                       std::string name,
                                       TypeReference type,
                                       std::unique_ptr<Expr> init,
                                       SourceSpan span) override;

  std::unique_ptr<StaticFieldDecl> MakeStaticField(bool is_public,
                                                   bool is_var,
                                                   std::string name,
                                                   TypeReference type,
                                                   std::unique_ptr<Expr> init,
                                                   SourceSpan span) override;

  std::unique_ptr<MethodDecl> MakeMethod(bool is_public,
                                         bool is_override,
                                         bool is_static,
                                         bool is_pure,
                                         std::string name,
                                         std::vector<Param> params,
                                         std::unique_ptr<TypeReference> ret_type,
                                         std::unique_ptr<Block> body,
                                         SourceSpan span) override;

  std::unique_ptr<CallDecl> MakeCallDecl(bool is_public,
                                         std::vector<Param> params,
                                         std::unique_ptr<TypeReference> ret_type,
                                         std::unique_ptr<Block> body,
                                         SourceSpan span) override;

  std::unique_ptr<DestructorDecl> MakeDestructor(bool is_public, std::unique_ptr<Block> body, SourceSpan span) override;

  // Statements
  std::unique_ptr<Block> MakeBlock(std::vector<std::unique_ptr<Stmt>> stmts, SourceSpan span) override;

  std::unique_ptr<VarDeclStmt> MakeVarDeclStmt(
      bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) override;

  std::unique_ptr<ExprStmt> MakeExprStmt(std::unique_ptr<Expr> expr, SourceSpan span) override;

  std::unique_ptr<ReturnStmt> MakeReturnStmt(std::unique_ptr<Expr> value, SourceSpan span) override;

  std::unique_ptr<BreakStmt> MakeBreakStmt(SourceSpan span) override;
  std::unique_ptr<ContinueStmt> MakeContinueStmt(SourceSpan span) override;

  std::unique_ptr<IfStmt> MakeIfStmt(std::vector<Branch> branches,
                                     std::unique_ptr<Block> else_block,
                                     SourceSpan span) override;

  std::unique_ptr<WhileStmt> MakeWhileStmt(std::unique_ptr<Expr> cond,
                                           std::unique_ptr<Block> body,
                                           SourceSpan span) override;

  std::unique_ptr<ForStmt> MakeForStmt(std::string iter_name,
                                       std::unique_ptr<Expr> iter_expr,
                                       std::unique_ptr<Block> body,
                                       SourceSpan span) override;

  std::unique_ptr<UnsafeBlock> MakeUnsafeBlock(std::unique_ptr<Block> body, SourceSpan span) override;

  // Expressions
  std::unique_ptr<Binary> MakeBinary(const IBinaryOpTag& op,
                                     std::unique_ptr<Expr> lhs,
                                     std::unique_ptr<Expr> rhs,
                                     SourceSpan span) override;

  std::unique_ptr<Unary> MakeUnary(const IUnaryOpTag& op, std::unique_ptr<Expr> operand, SourceSpan span) override;

  std::unique_ptr<Assign> MakeAssign(const IAssignOpTag& op,
                                     std::unique_ptr<Expr> target,
                                     std::unique_ptr<Expr> value,
                                     SourceSpan span) override;

  std::unique_ptr<Call> MakeCall(std::unique_ptr<Expr> callee,
                                 std::vector<std::unique_ptr<Expr>> args,
                                 SourceSpan span) override;

  std::unique_ptr<FieldAccess> MakeFieldAccess(std::unique_ptr<Expr> object,
                                               std::string name,
                                               SourceSpan span) override;

  std::unique_ptr<IndexAccess> MakeIndexAccess(std::unique_ptr<Expr> object,
                                               std::unique_ptr<Expr> index,
                                               SourceSpan span) override;

  std::unique_ptr<NamespaceRef> MakeNamespaceRef(std::unique_ptr<Expr> ns, std::string name, SourceSpan span) override;

  std::unique_ptr<SafeCall> MakeSafeCall(std::unique_ptr<Expr> object,
                                         std::string method,
                                         std::vector<std::unique_ptr<Expr>> args,
                                         std::optional<TypeReference> inferred_type,
                                         SourceSpan span) override;

  std::unique_ptr<Elvis> MakeElvis(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs, SourceSpan span) override;

  std::unique_ptr<CastAs> MakeCastAs(std::unique_ptr<Expr> expr, TypeReference type, SourceSpan span) override;

  std::unique_ptr<TypeTestIs> MakeTypeTestIs(std::unique_ptr<Expr> expr, TypeReference type, SourceSpan span) override;

  std::unique_ptr<IdentRef> MakeIdent(std::string name, SourceSpan span) override;
  std::unique_ptr<IntLit> MakeInt(long long v, SourceSpan span) override;
  std::unique_ptr<FloatLit> MakeFloat(long double v, SourceSpan span) override;
  std::unique_ptr<StringLit> MakeString(std::string v, SourceSpan span) override;
  std::unique_ptr<CharLit> MakeChar(char v, SourceSpan span) override;
  std::unique_ptr<BoolLit> MakeBool(bool v, SourceSpan span) override;
  std::unique_ptr<NullLit> MakeNull(SourceSpan span) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BUILDERASTFACTORY_HPP_
