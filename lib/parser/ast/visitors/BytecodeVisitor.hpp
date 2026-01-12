#ifndef PARSER_BYTECODEVISITOR_HPP_
#define PARSER_BYTECODEVISITOR_HPP_

#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

class BytecodeVisitor : public AstVisitor {
public:
  explicit BytecodeVisitor(std::ostream& output);
  ~BytecodeVisitor() override = default;

  BytecodeVisitor(const BytecodeVisitor&) = delete;
  BytecodeVisitor& operator=(const BytecodeVisitor&) = delete;
  BytecodeVisitor(BytecodeVisitor&&) = delete;
  BytecodeVisitor& operator=(BytecodeVisitor&&) = delete;

  void EmitThenStart();
  void EmitElseStart();
  void EmitElseIfStart();
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
  void Visit(ByteLit& node) override;
  void Visit(NullLit& node) override;
  void Visit(ThisExpr& node) override;

private:
  std::ostream& output_;

  int indent_level_{0};
  static constexpr const char* kIndent = "  ";

  std::string current_class_name_;
  std::string current_function_name_;
  std::vector<std::string> current_namespace_;

  std::unordered_map<std::string, size_t> local_variables_;
  std::unordered_map<std::string, size_t> static_variables_;
  std::unordered_map<std::string, std::string> variable_types_;
  size_t next_local_index_{0};
  size_t next_static_index_{0};

  std::unordered_map<std::string, std::string> function_name_map_;
  std::unordered_map<std::string, std::string> function_return_types_;
  size_t next_function_id_{0};

  std::vector<Expr*> pending_init_static_;
  std::vector<std::string> pending_init_static_names_;
  std::unordered_map<std::string, std::string> method_name_map_;
  std::unordered_map<std::string, std::string> method_vtable_map_;
  std::unordered_map<std::string, std::string> method_return_types_;
  std::unordered_map<std::string, std::vector<std::pair<std::string, TypeReference>>> class_fields_;
  std::unordered_map<std::string, std::vector<TypeReference>> constructor_params_;
  std::unordered_map<std::string, TypeReference> type_aliases_;

  static const std::unordered_set<std::string> kBuiltinSystemCommands;
  static const std::unordered_map<std::string, std::string> kBuiltinReturnPrimitives;
  static const std::unordered_set<std::string> kBuiltinTypeNames;
  static const std::unordered_set<std::string> kPrimitiveTypeNames;
  static const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> kBuiltinMethods;

  void EmitIndent() const;
  void EmitCommand(const std::string& command);
  void EmitCommandWithInt(const std::string& command, int64_t value);
  void EmitCommandWithFloat(const std::string& command, double value);
  void EmitCommandWithBool(const std::string& command, bool value);
  void EmitCommandWithString(const std::string& command, const std::string& value);
  void EmitCommandWithStringWithoutBraces(const std::string& command, const std::string& value);

  void EmitPushByte(uint8_t value);
  void EmitBlockStart();
  void EmitBlockStartWithoutSpaces();
  void EmitBlockEnd();
  void EmitBlockEndWithoutEscape();
  void EmitIfStart();

  std::string GenerateFunctionId(const std::string& name, const std::vector<Param>& params);

  std::string GenerateMethodId(const std::string& class_name,
                               const std::string& method_name,
                               const std::vector<Param>& params,
                               bool is_constructor,
                               bool is_destructor,
                               bool is_mutable = false);

  std::string GenerateMethodVTableName(const std::string& method_name,
                                       const std::vector<Param>& params,
                                       bool is_mutable = false);
  std::string GenerateConstructorId(const std::string& class_name, const std::vector<Param>& params);
  std::string GenerateDestructorId(const std::string& class_name);
  std::string GenerateCopyMethodId(const std::string& class_name, const std::string& param_type);
  std::string TypeToMangledName(const TypeReference& type);
  void VisitExpression(Expr* expr);
  void VisitStatement(Stmt* stmt);
  void VisitBlock(Block* block);

  size_t GetLocalIndex(const std::string& name);
  size_t GetStaticIndex(const std::string& name);
  void ResetLocalVariables();

  enum class OperandType : std::uint8_t { kInt, kFloat, kByte, kBool, kChar, kString, kUnknown };
  OperandType DetermineOperandType(Expr* expr);
  std::string GetOperandTypeName(Expr* expr);
  std::string GetTypeNameForExpr(Expr* expr);

  static bool IsPrimitiveWrapper(const std::string& type_name);
  bool IsPrimitiveType(const std::string& type_name) const;
  std::string GetPrimitiveTypeForWrapper(const std::string& wrapper_type) const;
  std::string GetWrapperTypeForPrimitive(const std::string& primitive_type) const;
  void EmitUnwrapIfNeeded(const std::string& type_name);
  void EmitWrapIfNeeded(const std::string& expected_type);

  void EmitTypeConversionIfNeeded(const std::string& expected_type, const std::string& actual_type);
  void EmitWrapConstructorCall(const std::string& wrapper_type, const std::string& primitive_type);
  int FindFieldIndex(const std::string& class_name, const std::string& field_name);
  std::string GetFieldTypeName(const std::string& class_name, const std::string& field_name);
  void EmitBinaryOperatorCommand(const IBinaryOpTag& op, OperandType dominant_type);
  static std::string GenerateArrayLengthMethodName(const std::string& array_type);
  std::string GenerateArrayGetAtMethodName(const std::string& array_type);
  std::string GenerateArraySetAtMethodName(const std::string& array_type);
  std::string GenerateArrayMethodName(const std::string& array_type, const std::string& method_name);
  std::string GetElementTypeForArray(const std::string& array_type);
  bool IsBuiltinSystemCommand(const std::string& name) const;
  void EmitParameterConversions(const std::vector<std::unique_ptr<Expr>>& args,
                                const std::vector<TypeReference>& expected_types);
  void EmitArgumentsInReverse(const std::vector<std::unique_ptr<Expr>>& args);
};

} // namespace ovum::compiler::parser

#endif
