#include "BuilderAstFactory.hpp"

#include <utility>

#include "nodes/builders/base/ParserBuilder.hpp"

namespace ovum::compiler::parser {

// Decls / Module

std::unique_ptr<Module> BuilderAstFactory::MakeModule(std::string name,
                                                      SourceId source_id,
                                                      std::vector<std::unique_ptr<Decl>> decls,
                                                      SourceSpan span) {
  auto b = ParserBuilder::Make<Module>();
  b.WithName(std::move(name)).WithSource(source_id).WithDecls(std::move(decls)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<FunctionDecl> BuilderAstFactory::MakeFunction(bool is_pure,
                                                              std::string name,
                                                              std::vector<Param> params,
                                                              std::unique_ptr<TypeReference> return_type,
                                                              std::unique_ptr<Block> body,
                                                              SourceSpan span) {
  auto b = ParserBuilder::Make<FunctionDecl>();
  b.WithPure(is_pure)
      .WithName(std::move(name))
      .WithParams(std::move(params))
      .WithReturnType(std::move(return_type))
      .WithBody(std::move(body))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<ClassDecl> BuilderAstFactory::MakeClass(std::string name,
                                                        std::vector<TypeReference> implements,
                                                        std::vector<std::unique_ptr<Decl>> members,
                                                        SourceSpan span) {
  auto b = ParserBuilder::Make<ClassDecl>();
  b.WithName(std::move(name)).WithImplements(std::move(implements)).WithMembers(std::move(members)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<InterfaceDecl> BuilderAstFactory::MakeInterface(std::string name,
                                                                std::vector<std::unique_ptr<InterfaceMethod>> methods,
                                                                SourceSpan span) {
  auto b = ParserBuilder::Make<InterfaceDecl>();
  b.WithName(std::move(name)).WithMethods(std::move(methods)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<InterfaceMethod> BuilderAstFactory::MakeInterfaceMethod(std::string name,
                                                                        std::vector<Param> params,
                                                                        std::unique_ptr<TypeReference> return_type,
                                                                        SourceSpan span) {
  auto b = ParserBuilder::Make<InterfaceMethod>();
  b.WithName(std::move(name)).WithParams(std::move(params)).WithReturnType(std::move(return_type)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<TypeAliasDecl> BuilderAstFactory::MakeTypeAlias(std::string name,
                                                                TypeReference aliased_type,
                                                                SourceSpan span) {
  auto b = ParserBuilder::Make<TypeAliasDecl>();
  b.WithName(std::move(name)).WithAliasedType(std::move(aliased_type)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<GlobalVarDecl> BuilderAstFactory::MakeGlobalVar(
    bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) {
  auto b = ParserBuilder::Make<GlobalVarDecl>();
  b.WithIsVar(is_var).WithName(std::move(name)).WithType(std::move(type)).WithInit(std::move(init)).WithSpan(span);
  return b.Build();
}

// Class members

std::unique_ptr<FieldDecl> BuilderAstFactory::MakeField(
    bool is_public, bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) {
  auto b = ParserBuilder::Make<FieldDecl>();
  b.WithIsPublic(is_public)
      .WithIsVar(is_var)
      .WithName(std::move(name))
      .WithType(std::move(type))
      .WithInit(std::move(init))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<StaticFieldDecl> BuilderAstFactory::MakeStaticField(
    bool is_public, bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) {
  auto b = ParserBuilder::Make<StaticFieldDecl>();
  b.WithIsPublic(is_public)
      .WithIsVar(is_var)
      .WithName(std::move(name))
      .WithType(std::move(type))
      .WithInit(std::move(init))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<MethodDecl> BuilderAstFactory::MakeMethod(bool is_public,
                                                          bool is_override,
                                                          bool is_static,
                                                          bool is_pure,
                                                          std::string name,
                                                          std::vector<Param> params,
                                                          std::unique_ptr<TypeReference> ret_type,
                                                          std::unique_ptr<Block> body,
                                                          SourceSpan span) {
  auto b = ParserBuilder::Make<MethodDecl>();
  b.WithIsPublic(is_public)
      .WithIsOverride(is_override)
      .WithIsStatic(is_static)
      .WithIsPure(is_pure)
      .WithName(std::move(name))
      .WithParams(std::move(params))
      .WithReturnType(std::move(ret_type))
      .WithBody(std::move(body))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<CallDecl> BuilderAstFactory::MakeCallDecl(bool is_public,
                                                          std::vector<Param> params,
                                                          std::unique_ptr<TypeReference> ret_type,
                                                          std::unique_ptr<Block> body,
                                                          SourceSpan span) {
  auto b = ParserBuilder::Make<CallDecl>();
  b.WithIsPublic(is_public)
      .WithParams(std::move(params))
      .WithReturnType(std::move(ret_type))
      .WithBody(std::move(body))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<DestructorDecl> BuilderAstFactory::MakeDestructor(bool is_public,
                                                                  std::unique_ptr<Block> body,
                                                                  SourceSpan span) {
  auto b = ParserBuilder::Make<DestructorDecl>();
  b.WithIsPublic(is_public).WithBody(std::move(body)).WithSpan(span);
  return b.Build();
}

// Statements

std::unique_ptr<Block> BuilderAstFactory::MakeBlock(std::vector<std::unique_ptr<Stmt>> stmts, SourceSpan span) {
  auto b = ParserBuilder::Make<Block>();
  b.WithStatements(std::move(stmts)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<VarDeclStmt> BuilderAstFactory::MakeVarDeclStmt(
    bool is_var, std::string name, TypeReference type, std::unique_ptr<Expr> init, SourceSpan span) {
  auto b = ParserBuilder::Make<VarDeclStmt>();
  b.WithIsVar(is_var).WithName(std::move(name)).WithType(std::move(type)).WithInit(std::move(init)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<ExprStmt> BuilderAstFactory::MakeExprStmt(std::unique_ptr<Expr> expr, SourceSpan span) {
  auto b = ParserBuilder::Make<ExprStmt>();
  b.WithExpr(std::move(expr)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<ReturnStmt> BuilderAstFactory::MakeReturnStmt(std::unique_ptr<Expr> value, SourceSpan span) {
  auto b = ParserBuilder::Make<ReturnStmt>();
  b.WithValue(std::move(value)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<BreakStmt> BuilderAstFactory::MakeBreakStmt(SourceSpan span) {
  auto b = ParserBuilder::Make<BreakStmt>();
  b.WithSpan(span);
  return b.Build();
}

std::unique_ptr<ContinueStmt> BuilderAstFactory::MakeContinueStmt(SourceSpan span) {
  auto b = ParserBuilder::Make<ContinueStmt>();
  b.WithSpan(span);
  return b.Build();
}

std::unique_ptr<IfStmt> BuilderAstFactory::MakeIfStmt(std::vector<Branch> branches,
                                                      std::unique_ptr<Block> else_block,
                                                      SourceSpan span) {
  auto b = ParserBuilder::Make<IfStmt>();
  b.WithBranches(std::move(branches)).WithElse(std::move(else_block)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<WhileStmt> BuilderAstFactory::MakeWhileStmt(std::unique_ptr<Expr> cond,
                                                            std::unique_ptr<Block> body,
                                                            SourceSpan span) {
  auto b = ParserBuilder::Make<WhileStmt>();
  b.WithCondition(std::move(cond)).WithBody(std::move(body)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<ForStmt> BuilderAstFactory::MakeForStmt(std::string iter_name,
                                                        std::unique_ptr<Expr> iter_expr,
                                                        std::unique_ptr<Block> body,
                                                        SourceSpan span) {
  auto b = ParserBuilder::Make<ForStmt>();
  b.WithIteratorName(std::move(iter_name))
      .WithIteratorExpr(std::move(iter_expr))
      .WithBody(std::move(body))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<UnsafeBlock> BuilderAstFactory::MakeUnsafeBlock(std::unique_ptr<Block> body, SourceSpan span) {
  auto b = ParserBuilder::Make<UnsafeBlock>();
  b.WithBody(std::move(body)).WithSpan(span);
  return b.Build();
}

// Expressions

std::unique_ptr<Binary> BuilderAstFactory::MakeBinary(const IBinaryOpTag& op,
                                                      std::unique_ptr<Expr> lhs,
                                                      std::unique_ptr<Expr> rhs,
                                                      SourceSpan span) {
  auto b = ParserBuilder::Make<Binary>();
  b.WithOp(op).WithLhs(std::move(lhs)).WithRhs(std::move(rhs)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<Unary> BuilderAstFactory::MakeUnary(const IUnaryOpTag& op,
                                                    std::unique_ptr<Expr> operand,
                                                    SourceSpan span) {
  auto b = ParserBuilder::Make<Unary>();
  b.WithOp(op).WithOperand(std::move(operand)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<Assign> BuilderAstFactory::MakeAssign(const IAssignOpTag& op,
                                                      std::unique_ptr<Expr> target,
                                                      std::unique_ptr<Expr> value,
                                                      SourceSpan span) {
  auto b = ParserBuilder::Make<Assign>();
  b.WithKind(op).WithTarget(std::move(target)).WithValue(std::move(value)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<Call> BuilderAstFactory::MakeCall(std::unique_ptr<Expr> callee,
                                                  std::vector<std::unique_ptr<Expr>> args,
                                                  SourceSpan span) {
  auto b = ParserBuilder::Make<Call>();
  b.WithCallee(std::move(callee)).WithArgs(std::move(args)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<FieldAccess> BuilderAstFactory::MakeFieldAccess(std::unique_ptr<Expr> object,
                                                                std::string name,
                                                                SourceSpan span) {
  auto b = ParserBuilder::Make<FieldAccess>();
  b.WithObject(std::move(object)).WithName(std::move(name)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<IndexAccess> BuilderAstFactory::MakeIndexAccess(std::unique_ptr<Expr> object,
                                                                std::unique_ptr<Expr> index,
                                                                SourceSpan span) {
  auto b = ParserBuilder::Make<IndexAccess>();
  b.WithObject(std::move(object)).WithIndex(std::move(index)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<NamespaceRef> BuilderAstFactory::MakeNamespaceRef(std::unique_ptr<Expr> ns,
                                                                  std::string name,
                                                                  SourceSpan span) {
  auto b = ParserBuilder::Make<NamespaceRef>();
  b.WithNamespace(std::move(ns)).WithName(std::move(name)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<SafeCall> BuilderAstFactory::MakeSafeCall(std::unique_ptr<Expr> object,
                                                          std::string method,
                                                          std::vector<std::unique_ptr<Expr>> args,
                                                          std::optional<TypeReference> inferred_type,
                                                          SourceSpan span) {
  auto b = ParserBuilder::Make<SafeCall>();
  b.WithObject(std::move(object))
      .WithMethod(std::move(method))
      .WithArgs(std::move(args))
      .WithInferredType(std::move(inferred_type))
      .WithSpan(span);
  return b.Build();
}

std::unique_ptr<Elvis> BuilderAstFactory::MakeElvis(std::unique_ptr<Expr> lhs,
                                                    std::unique_ptr<Expr> rhs,
                                                    SourceSpan span) {
  auto b = ParserBuilder::Make<Elvis>();
  b.WithLhs(std::move(lhs)).WithRhs(std::move(rhs)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<CastAs> BuilderAstFactory::MakeCastAs(std::unique_ptr<Expr> expr, TypeReference type, SourceSpan span) {
  auto b = ParserBuilder::Make<CastAs>();
  b.WithExpr(std::move(expr)).WithType(std::move(type)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<TypeTestIs> BuilderAstFactory::MakeTypeTestIs(std::unique_ptr<Expr> expr,
                                                              TypeReference type,
                                                              SourceSpan span) {
  auto b = ParserBuilder::Make<TypeTestIs>();
  b.WithExpr(std::move(expr)).WithType(std::move(type)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<IdentRef> BuilderAstFactory::MakeIdent(std::string name, SourceSpan span) {
  auto b = ParserBuilder::Make<IdentRef>();
  b.WithName(std::move(name)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<IntLit> BuilderAstFactory::MakeInt(long long v, SourceSpan span) {
  auto b = ParserBuilder::Make<IntLit>();
  b.WithValue(v).WithSpan(span);
  return b.Build();
}

std::unique_ptr<FloatLit> BuilderAstFactory::MakeFloat(long double v, SourceSpan span) {
  auto b = ParserBuilder::Make<FloatLit>();
  b.WithValue(v).WithSpan(span);
  return b.Build();
}

std::unique_ptr<StringLit> BuilderAstFactory::MakeString(std::string v, SourceSpan span) {
  auto b = ParserBuilder::Make<StringLit>();
  b.WithValue(std::move(v)).WithSpan(span);
  return b.Build();
}

std::unique_ptr<CharLit> BuilderAstFactory::MakeChar(char v, SourceSpan span) {
  auto b = ParserBuilder::Make<CharLit>();
  b.WithValue(v).WithSpan(span);
  return b.Build();
}

std::unique_ptr<BoolLit> BuilderAstFactory::MakeBool(bool v, SourceSpan span) {
  auto b = ParserBuilder::Make<BoolLit>();
  b.WithValue(v).WithSpan(span);
  return b.Build();
}

std::unique_ptr<NullLit> BuilderAstFactory::MakeNull(SourceSpan span) {
  auto b = ParserBuilder::Make<NullLit>();
  b.WithSpan(span);
  return b.Build();
}

} // namespace ovum::compiler::parser
