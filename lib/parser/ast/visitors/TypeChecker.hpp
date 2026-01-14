#ifndef PARSER_TYPECHECKER_HPP_
#define PARSER_TYPECHECKER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "WalkVisitor.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class TypeChecker : public WalkVisitor {
public:
  explicit TypeChecker(IDiagnosticSink& sink) : sink_(sink) {
  }

  void Visit(Module& node) override;
  void Visit(FunctionDecl& node) override;
  void Visit(ClassDecl& node) override;
  void Visit(MethodDecl& node) override;
  void Visit(VarDeclStmt& node) override;
  void Visit(GlobalVarDecl& node) override;
  void Visit(FieldDecl& node) override;
  void Visit(ReturnStmt& node) override;
  void Visit(Assign& node) override;
  void Visit(Call& node) override;
  void Visit(FieldAccess& node) override;
  void Visit(IndexAccess& node) override;
  void Visit(Binary& node) override;
  void Visit(Unary& node) override;
  void Visit(SafeCall& node) override;
  void Visit(Elvis& node) override;
  void Visit(CastAs& node) override;
  void Visit(TypeTestIs& node) override;
  void Visit(InterfaceDecl& node) override;
  void Visit(IdentRef& node) override;
  void Visit(ExprStmt& node) override;

private:
  struct FunctionSignature {
    std::string name;
    std::vector<TypeReference> param_types;
    std::unique_ptr<TypeReference> return_type;
  };

  struct FunctionOverload {
    std::vector<TypeReference> param_types;
    std::unique_ptr<TypeReference> return_type;
  };

  struct MethodSignature {
    std::string class_name;
    std::string method_name;
    std::vector<TypeReference> param_types;
    std::unique_ptr<TypeReference> return_type;
  };

  struct BuiltinMethodSignature {
    std::vector<TypeReference> param_types;
    std::unique_ptr<TypeReference> return_type;
  };

  TypeReference* ResolveTypeAlias(const TypeReference& type);
  TypeReference InferExpressionType(Expr* expr);
  bool TypesCompatible(const TypeReference& expected, const TypeReference& actual);
  bool IsSameType(const TypeReference& a, const TypeReference& b);
  bool IsPrimitiveWrapper(const std::string& type_name) const;
  bool IsPrimitiveType(const std::string& type_name) const;
  bool IsClassType(const std::string& type_name) const;
  bool IsImplicitlyConvertible(const TypeReference& from, const TypeReference& to);
  const FunctionOverload* ResolveFunctionOverload(const std::string& func_name,
                                                  const std::vector<std::unique_ptr<Expr>>& args);
  const BuiltinMethodSignature* FindBuiltinMethod(const std::string& type_name, const std::string& method_name);
  std::string GetFundamentalTypeName(const TypeReference& type);
  TypeReference GetElementTypeForArray(const TypeReference& array_type);
  void InitializeBuiltinMethods();

  IDiagnosticSink& sink_;
  TypeReference* current_return_type_ = nullptr;
  std::string current_class_name_;

  struct InterfaceSignature {
    std::string interface_name;
    std::unordered_map<std::string, MethodSignature> methods;
  };

  std::unordered_map<std::string, TypeReference> variable_types_;
  std::unordered_map<std::string, FunctionSignature> functions_; // Kept for backward compatibility
  std::unordered_map<std::string, std::vector<FunctionOverload>> function_overloads_;
  std::unordered_map<std::string, MethodSignature> methods_;
  std::unordered_map<std::string, std::vector<std::pair<std::string, TypeReference>>> class_fields_;
  std::unordered_map<std::string, InterfaceSignature> interfaces_;
  std::unordered_map<std::string, std::vector<std::string>> class_implements_; // class_name -> list of interface names
  std::unordered_map<std::string, TypeReference> type_aliases_;
  std::unordered_map<std::string, TypeReference> global_variables_;
  std::unordered_map<std::string, MethodSignature> constructors_;
  std::unordered_map<std::string, std::unordered_map<std::string, BuiltinMethodSignature>> builtin_methods_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_TYPECHECKER_HPP_
