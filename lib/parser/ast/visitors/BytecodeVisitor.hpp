#ifndef PARSER_BYTECODEVISITOR_HPP_
#define PARSER_BYTECODEVISITOR_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::vm::execution_tree {
class Block;
class Function;
class FunctionRepository;
class IExecutable;
}  // namespace ovum::vm::execution_tree

namespace ovum::compiler::parser {

class BytecodeVisitor : public AstVisitor {
public:
  explicit BytecodeVisitor(ovum::vm::execution_tree::FunctionRepository& function_repository);

  ~BytecodeVisitor() override = default;

  // Decls
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

  // Stmts
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

  // Exprs
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

  // Get generated functions
  [[nodiscard]] std::vector<std::unique_ptr<ovum::vm::execution_tree::Function>> ReleaseFunctions();

private:
  ovum::vm::execution_tree::FunctionRepository& function_repository_;

  std::unique_ptr<ovum::vm::execution_tree::Block> current_block_;
  std::string current_class_name_;
  std::vector<std::string> current_namespace_;

  // Variable tracking
  std::unordered_map<std::string, size_t> local_variables_;
  std::unordered_map<std::string, size_t> static_variables_;
  size_t next_local_index_{0};
  size_t next_static_index_{0};

  // Helper methods
  void PushCommand(std::unique_ptr<ovum::vm::execution_tree::IExecutable> cmd);
  std::string GenerateFunctionId(const std::string& name, const std::vector<Param>& params);
  std::string GenerateMethodId(const std::string& class_name, const std::string& method_name,
                               const std::vector<Param>& params, bool is_constructor, bool is_destructor);
  std::string GenerateConstructorId(const std::string& class_name, const std::vector<Param>& params);
  std::string GenerateDestructorId(const std::string& class_name);
  std::string TypeToMangledName(const TypeReference& type);
  void VisitExpression(Expr* expr);
  void VisitStatement(Stmt* stmt);
  void VisitBlock(Block* block);
  std::unique_ptr<ovum::vm::execution_tree::Block> CreateBlock();
  void SetCurrentBlock(std::unique_ptr<ovum::vm::execution_tree::Block> block);
  size_t GetLocalIndex(const std::string& name);
  size_t GetStaticIndex(const std::string& name);
  void ResetLocalVariables();
};

}  // namespace ovum::compiler::parser

#endif  // PARSER_BYTECODEVISITOR_HPP_

