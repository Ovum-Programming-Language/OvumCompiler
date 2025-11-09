#ifndef PARSER_LINTVISITOR_HPP_
#define PARSER_LINTVISITOR_HPP_

#include <cstddef>
#include <string>

#include "WalkVisitor.hpp"

#include "lib/parser/diagnostics/IDiagnosticSink.hpp"

namespace ovum::compiler::parser {

struct LintOptions {
  std::size_t max_block_len = 200;
  std::size_t max_nesting = 4;
  std::size_t max_class_members = 64;
  bool warn_empty_blocks = true;
  bool warn_public_fields = true;
  bool warn_mutable_globals = true;
  bool warn_static_mutable_fields = true;
  bool warn_unreachable = true;
  bool warn_pure_expr_stmt = true;
  bool warn_break_continue_outside_loop = true;
  bool warn_empty_bodies = true;
  bool warn_empty_else = true;
  bool warn_missing_loop_cond_or_iterable = true;
  bool warn_module_without_decls = true;
  bool warn_empty_string_literal = true;
  bool warn_deep_nesting = true;
  bool warn_large_class = true;
  bool warn_if_without_branches = true;
  bool warn_while_true = true;
};

class LintVisitor : public WalkVisitor {
public:
  explicit LintVisitor(IDiagnosticSink& sink, LintOptions options = {}) : sink_(sink), opts_(options) {
  }

  void Visit(Module& node) override;
  void Visit(ClassDecl& node) override;
  void Visit(FunctionDecl& node) override;
  void Visit(MethodDecl& node) override;
  void Visit(CallDecl& node) override;
  void Visit(DestructorDecl& node) override;

  void Visit(Block& node) override;
  void Visit(ExprStmt& node) override;
  void Visit(ReturnStmt& node) override;
  void Visit(BreakStmt& node) override;
  void Visit(ContinueStmt& node) override;
  void Visit(IfStmt& node) override;
  void Visit(WhileStmt& node) override;
  void Visit(ForStmt& node) override;
  void Visit(UnsafeBlock& node) override;

  void Visit(GlobalVarDecl& node) override;
  void Visit(FieldDecl& node) override;
  void Visit(StaticFieldDecl& node) override;

  void Visit(StringLit& node) override;
  void Visit(BoolLit& node) override;

private:
  bool IsPureExpr(Expr& expression) const;
  void EnterBody();
  void LeaveBody();
  void EnterLoop();
  void LeaveLoop();
  void CheckNestingDepth(const SourceSpan& where_hint = {}) const;

  IDiagnosticSink& sink_;
  LintOptions opts_;
  std::size_t loop_depth_ = 0;
  std::size_t nesting_depth_ = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_LINTVISITOR_HPP_
