#ifndef PARSER_BUILDERTRAITS_HPP_
#define PARSER_BUILDERTRAITS_HPP_

#include <type_traits>

#include "lib/parser/ast/nodes/builders/class_members/CallDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/class_members/DestructorDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/class_members/FieldDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/class_members/MethodDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/class_members/StaticFieldDeclBuilder.hpp"

#include "lib/parser/ast/nodes/builders/decls/ClassDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/FunctionDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/GlobalVarDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/InterfaceDeclBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/InterfaceMethodBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/ModuleBuilder.hpp"
#include "lib/parser/ast/nodes/builders/decls/TypeAliasDeclBuilder.hpp"

#include "lib/parser/ast/nodes/builders/exprs/AssignBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/BinaryBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/CallExprBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/CastAsBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/ElvisBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/FieldAccessBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/IdentRefBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/IndexAccessBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/NamespaceRefBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/SafeCallBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/TypeTestIsBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/UnaryBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/BoolLitBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/CharLitBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/FloatLitBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/IntLitBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/NullLitBuilder.hpp"
#include "lib/parser/ast/nodes/builders/exprs/literals/StringLitBuilder.hpp"

#include "lib/parser/ast/nodes/builders/stmts/BlockBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/BreakStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/ContinueStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/ExprStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/ForStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/IfStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/ReturnStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/UnsafeBlockBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/VarDeclStmtBuilder.hpp"
#include "lib/parser/ast/nodes/builders/stmts/WhileStmtBuilder.hpp"

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

template<class T>
struct BuilderFor;

// Decls
template<>
struct BuilderFor<Module> {
  using type = ModuleBuilder;
};

template<>
struct BuilderFor<FunctionDecl> {
  using type = FunctionDeclBuilder;
};

template<>
struct BuilderFor<ClassDecl> {
  using type = ClassDeclBuilder;
};

template<>
struct BuilderFor<GlobalVarDecl> {
  using type = GlobalVarDeclBuilder;
};

template<>
struct BuilderFor<InterfaceDecl> {
  using type = InterfaceDeclBuilder;
};

template<>
struct BuilderFor<InterfaceMethod> {
  using type = InterfaceMethodBuilder;
};

template<>
struct BuilderFor<TypeAliasDecl> {
  using type = TypeAliasDeclBuilder;
};

template<>
struct BuilderFor<FieldDecl> {
  using type = FieldDeclBuilder;
};

template<>
struct BuilderFor<StaticFieldDecl> {
  using type = StaticFieldDeclBuilder;
};

template<>
struct BuilderFor<MethodDecl> {
  using type = MethodDeclBuilder;
};

template<>
struct BuilderFor<CallDecl> {
  using type = CallDeclBuilder;
};

template<>
struct BuilderFor<DestructorDecl> {
  using type = DestructorDeclBuilder;
};

// Stmts
template<>
struct BuilderFor<Block> {
  using type = BlockBuilder;
};

template<>
struct BuilderFor<BreakStmt> {
  using type = BreakStmtBuilder;
};

template<>
struct BuilderFor<ContinueStmt> {
  using type = ContinueStmtBuilder;
};

template<>
struct BuilderFor<ExprStmt> {
  using type = ExprStmtBuilder;
};

template<>
struct BuilderFor<ForStmt> {
  using type = ForStmtBuilder;
};

template<>
struct BuilderFor<IfStmt> {
  using type = IfStmtBuilder;
};

template<>
struct BuilderFor<ReturnStmt> {
  using type = ReturnStmtBuilder;
};

template<>
struct BuilderFor<UnsafeBlock> {
  using type = UnsafeBlockBuilder;
};

template<>
struct BuilderFor<VarDeclStmt> {
  using type = VarDeclStmtBuilder;
};

template<>
struct BuilderFor<WhileStmt> {
  using type = WhileStmtBuilder;
};

// Exprs
template<>
struct BuilderFor<Assign> {
  using type = AssignBuilder;
};

template<>
struct BuilderFor<Binary> {
  using type = BinaryBuilder;
};

template<>
struct BuilderFor<Call> {
  using type = CallExprBuilder;
};

template<>
struct BuilderFor<CastAs> {
  using type = CastAsBuilder;
};

template<>
struct BuilderFor<Elvis> {
  using type = ElvisBuilder;
};

template<>
struct BuilderFor<FieldAccess> {
  using type = FieldAccessBuilder;
};

template<>
struct BuilderFor<IdentRef> {
  using type = IdentRefBuilder;
};

template<>
struct BuilderFor<IndexAccess> {
  using type = IndexAccessBuilder;
};

template<>
struct BuilderFor<NamespaceRef> {
  using type = NamespaceRefBuilder;
};

template<>
struct BuilderFor<SafeCall> {
  using type = SafeCallBuilder;
};

template<>
struct BuilderFor<TypeTestIs> {
  using type = TypeTestIsBuilder;
};

template<>
struct BuilderFor<Unary> {
  using type = UnaryBuilder;
};

// Literals
template<>
struct BuilderFor<BoolLit> {
  using type = BoolLitBuilder;
};

template<>
struct BuilderFor<CharLit> {
  using type = CharLitBuilder;
};

template<>
struct BuilderFor<FloatLit> {
  using type = FloatLitBuilder;
};

template<>
struct BuilderFor<IntLit> {
  using type = IntLitBuilder;
};

template<>
struct BuilderFor<NullLit> {
  using type = NullLitBuilder;
};

template<>
struct BuilderFor<StringLit> {
  using type = StringLitBuilder;
};

template<class T, class = void>
struct HasBuilder : std::false_type {};

template<class T>
struct HasBuilder<T, std::void_t<typename BuilderFor<T>::type>> : std::true_type {};

template<class T>
inline constexpr bool kHasBuilderV = HasBuilder<T>::value;

} // namespace ovum::compiler::parser

#endif // PARSER_BUILDERTRAITS_HPP_
