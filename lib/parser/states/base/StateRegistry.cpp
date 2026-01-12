#include "StateRegistry.hpp"

#include "lib/parser/states/StateBlock.hpp"
#include "lib/parser/states/StateCallDeclHdr.hpp"
#include "lib/parser/states/StateDestructorDecl.hpp"
#include "lib/parser/states/StateExpr.hpp"
#include "lib/parser/states/StateFieldDecl.hpp"
#include "lib/parser/states/StateForHead.hpp"
#include "lib/parser/states/StateGlobalVarDecl.hpp"
#include "lib/parser/states/StateIfHead.hpp"
#include "lib/parser/states/StateIfTail.hpp"
#include "lib/parser/states/StateMethodHdr.hpp"
#include "lib/parser/states/StateModule.hpp"
#include "lib/parser/states/StateParseType.hpp"
#include "lib/parser/states/StateReturnTail.hpp"
#include "lib/parser/states/StateStmt.hpp"
#include "lib/parser/states/StateSyncToBlockEnd.hpp"
#include "lib/parser/states/StateSyncToStmtEnd.hpp"
#include "lib/parser/states/StateTopDecl.hpp"
#include "lib/parser/states/StateTypeAliasDecl.hpp"
#include "lib/parser/states/StateUnsafeBlock.hpp"
#include "lib/parser/states/StateVarDeclTail.hpp"
#include "lib/parser/states/StateWhileHead.hpp"
#include "lib/parser/states/class/StateClassBody.hpp"
#include "lib/parser/states/class/StateClassHdr.hpp"
#include "lib/parser/states/class/StateClassMember.hpp"
#include "lib/parser/states/func/StateFuncBody.hpp"
#include "lib/parser/states/func/StateFuncHdr.hpp"
#include "lib/parser/states/func/StateFuncParams.hpp"
#include "lib/parser/states/interface/StateInterfaceBody.hpp"
#include "lib/parser/states/interface/StateInterfaceDecl.hpp"
#include "lib/parser/states/interface/StateInterfaceHdr.hpp"

namespace ovum::compiler::parser {

const IState& StateRegistry::Module() {
  static StateModule s;
  return s;
}
const IState& StateRegistry::TopDecl() {
  static StateTopDecl s;
  return s;
}
const IState& StateRegistry::FuncHdr() {
  static StateFuncHdr s;
  return s;
}
const IState& StateRegistry::FuncParams() {
  static StateFuncParams s;
  return s;
}
const IState& StateRegistry::FuncBody() {
  static StateFuncBody s;
  return s;
}
const IState& StateRegistry::ClassHdr() {
  static StateClassHdr s;
  return s;
}
const IState& StateRegistry::ClassBody() {
  static StateClassBody s;
  return s;
}
const IState& StateRegistry::ClassMember() {
  static StateClassMember s;
  return s;
}
const IState& StateRegistry::InterfaceHdr() {
  static StateInterfaceHdr s;
  return s;
}
const IState& StateRegistry::InterfaceBody() {
  static StateInterfaceBody s;
  return s;
}
const IState& StateRegistry::InterfaceDecl() {
  static StateInterfaceDecl s;
  return s;
}

const IState& StateRegistry::SignatureDecl() {
  return InterfaceDecl(); // Alias for InterfaceDecl
}
const IState& StateRegistry::TypeAliasDecl() {
  static StateTypeAliasDecl s;
  return s;
}
const IState& StateRegistry::GlobalVarDecl() {
  static StateGlobalVarDecl s;
  return s;
}
const IState& StateRegistry::DestructorDecl() {
  static StateDestructorDecl s;
  return s;
}
const IState& StateRegistry::CallDeclHdr() {
  static StateCallDeclHdr s;
  return s;
}
const IState& StateRegistry::MethodHdr() {
  static StateMethodHdr s;
  return s;
}
const IState& StateRegistry::FieldDecl() {
  static StateFieldDecl s;
  return s;
}
const IState& StateRegistry::Block() {
  static StateBlock s;
  return s;
}
const IState& StateRegistry::Stmt() {
  static StateStmt s;
  return s;
}
const IState& StateRegistry::IfHead() {
  static StateIfHead s;
  return s;
}
const IState& StateRegistry::IfTail() {
  static StateIfTail s;
  return s;
}
const IState& StateRegistry::WhileHead() {
  static StateWhileHead s;
  return s;
}
const IState& StateRegistry::ForHead() {
  static StateForHead s;
  return s;
}
const IState& StateRegistry::ReturnTail() {
  static StateReturnTail s;
  return s;
}
const IState& StateRegistry::VarDeclTail() {
  static StateVarDeclTail s;
  return s;
}
const IState& StateRegistry::UnsafeBlock() {
  static StateUnsafeBlock s;
  return s;
}
const IState& StateRegistry::ParseType() {
  static StateParseType s;
  return s;
}
const IState& StateRegistry::Expr() {
  static StateExpr s;
  return s;
}
const IState& StateRegistry::SyncToStmtEnd() {
  static StateSyncToStmtEnd s;
  return s;
}
const IState& StateRegistry::SyncToBlockEnd() {
  static StateSyncToBlockEnd s;
  return s;
}

} // namespace ovum::compiler::parser
