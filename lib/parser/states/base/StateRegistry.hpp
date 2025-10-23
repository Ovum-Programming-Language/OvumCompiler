#ifndef STATEREGISTRY_HPP_
#define STATEREGISTRY_HPP_

#include "IState.hpp"

class StateRegistry {
  static const IState& Module();
  static const IState& TopDecl();

  static const IState& FuncHdr();
  static const IState& FuncParams();
  static const IState& FuncBody();

  static const IState& ClassHdr();
  static const IState& ClassBody();
  static const IState& ClassMember();
  static const IState& InterfaceHdr();
  static const IState& InterfaceBody();
  static const IState& SignatureDecl();

  static const IState& TypeAliasDecl();
  static const IState& GlobalVarDecl();

  static const IState& DestructorDecl();
  static const IState& CallDeclHdr();
  static const IState& MethodHdr();
  static const IState& FieldDecl();

  static const IState& Block();
  static const IState& Stmt();
  static const IState& IfHead();
  static const IState& IfTail();
  static const IState& WhileHead();
  static const IState& ForHead();
  static const IState& ReturnTail();
  static const IState& VarDeclTail();
  static const IState& UnsafeBlock();

  static const IState& ParseType();
  static const IState& Expr();
  static const IState& SyncToStmtEnd();
  static const IState& SyncToBlockEnd();
};

#endif // STATEREGISTRY_HPP_
