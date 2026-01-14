#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "TypeChecker.hpp"

#include <sstream>

#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
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
#include "lib/parser/ast/nodes/exprs/ThisExpr.hpp"
#include "lib/parser/ast/nodes/exprs/TypeTestIs.hpp"
#include "lib/parser/ast/nodes/exprs/Unary.hpp"
#include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/ByteLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/CharLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/FloatLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/IntLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/NullLit.hpp"
#include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"
#include "lib/parser/ast/nodes/exprs/tags/optags.hpp"
#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
#include "lib/parser/ast/nodes/stmts/VarDeclStmt.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "types/TypeReference.hpp"

namespace ovum::compiler::parser {

namespace {
const std::unordered_set<std::string> kBuiltinSystemCommands = {"Print",
                                                                "PrintLine",
                                                                "ReadLine",
                                                                "ReadChar",
                                                                "ReadInt",
                                                                "ReadFloat",
                                                                "UnixTime",
                                                                "UnixTimeMs",
                                                                "UnixTimeNs",
                                                                "NanoTime",
                                                                "FormatDateTime",
                                                                "ParseDateTime",
                                                                "FileExists",
                                                                "DirectoryExists",
                                                                "CreateDirectory",
                                                                "DeleteFile",
                                                                "DeleteDirectory",
                                                                "MoveFile",
                                                                "CopyFile",
                                                                "ListDirectory",
                                                                "GetCurrentDirectory",
                                                                "ChangeDirectory",
                                                                "SleepMs",
                                                                "SleepNs",
                                                                "Exit",
                                                                "GetProcessId",
                                                                "GetEnvironmentVar",
                                                                "SetEnvironmentVar",
                                                                "Random",
                                                                "RandomRange",
                                                                "RandomFloat",
                                                                "RandomFloatRange",
                                                                "SeedRandom",
                                                                "GetMemoryUsage",
                                                                "GetPeakMemoryUsage",
                                                                "ForceGarbageCollection",
                                                                "GetProcessorCount",
                                                                "GetOsName",
                                                                "GetOsVersion",
                                                                "GetArchitecture",
                                                                "GetUserName",
                                                                "GetHomeDirectory",
                                                                "Interop",
                                                                "TypeOf"};

const std::unordered_set<std::string> kBuiltinTypeNames = {"Int",
                                                           "Float",
                                                           "String",
                                                           "Bool",
                                                           "Char",
                                                           "Byte",
                                                           "IntArray",
                                                           "FloatArray",
                                                           "StringArray",
                                                           "BoolArray",
                                                           "ByteArray",
                                                           "CharArray",
                                                           "ObjectArray",
                                                           "File"};

const std::unordered_map<std::string, std::string> kBuiltinReturnTypes = {
    // Random functions
    {"RandomFloatRange", "Float"},
    {"RandomFloat", "Float"},
    {"Random", "Int"},
    {"RandomRange", "Int"},
    // Time functions
    {"UnixTime", "Int"},
    {"UnixTimeMs", "Int"},
    {"UnixTimeNs", "Int"},
    {"NanoTime", "Int"},
    // Process functions
    {"GetProcessId", "Int"},
    {"GetMemoryUsage", "Int"},
    {"GetPeakMemoryUsage", "Int"},
    {"GetProcessorCount", "Int"},
    // Date/Time functions
    {"ParseDateTime", "Int"},
    {"FormatDateTime", "String"},
    // File operations
    {"FileExists", "Bool"},
    {"DirectoryExists", "Bool"},
    {"CreateDirectory", "Bool"},
    {"DeleteFile", "Bool"},
    {"DeleteDirectory", "Bool"},
    {"MoveFile", "Bool"},
    {"CopyFile", "Bool"},
    {"ListDirectory", "StringArray"},
    {"GetCurrentDirectory", "String"},
    {"ChangeDirectory", "Bool"},
    // I/O functions
    {"ReadLine", "String"},
    {"ReadChar", "Char"},
    {"ReadInt", "Int"},
    {"ReadFloat", "Float"},
    // System information
    {"GetOsName", "String"},
    {"GetOsVersion", "String"},
    {"GetArchitecture", "String"},
    {"GetUserName", "String"},
    {"GetHomeDirectory", "String"},
    // Environment
    {"SetEnvironmentVar", "Bool"},
    // Math functions
    {"Sqrt", "float"},
    {"ToString", "String"},
    {"ToInt", "int"},
    {"ToFloat", "float"},
    // Type functions
    {"TypeOf", "String"},
    // FFI
    {"Interop", "int"},
};
} // namespace

void TypeChecker::Visit(Module& node) {
  functions_.clear();
  function_overloads_.clear();
  methods_.clear();
  class_fields_.clear();
  interfaces_.clear();
  class_implements_.clear();
  type_aliases_.clear();
  global_variables_.clear();
  constructors_.clear();
  builtin_methods_.clear();

  // Initialize builtin methods
  InitializeBuiltinMethods();

  for (auto& decl : node.MutableDecls()) {
    if (auto* f = dynamic_cast<FunctionDecl*>(decl.get())) {
      FunctionSignature sig;
      sig.name = f->Name();
      for (const auto& param : f->Params()) {
        sig.param_types.emplace_back(param.GetType());
      }
      if (f->ReturnType() != nullptr) {
        sig.return_type = std::make_unique<TypeReference>(*f->ReturnType());
      }
      functions_[f->Name()] = std::move(sig);

      // Also register in function_overloads_
      FunctionOverload overload;
      for (const auto& param : f->Params()) {
        overload.param_types.push_back(param.GetType());
      }
      if (f->ReturnType() != nullptr) {
        overload.return_type = std::make_unique<TypeReference>(*f->ReturnType());
      }
      function_overloads_[f->Name()].push_back(std::move(overload));
    }

    if (auto* c = dynamic_cast<ClassDecl*>(decl.get())) {
      std::string class_name = c->Name();
      std::vector<std::pair<std::string, TypeReference>> fields;

      // Register interfaces that this class implements
      std::vector<std::string> interface_names;
      for (const auto& impl : c->Implements()) {
        if (!impl.QualifiedName().empty()) {
          interface_names.emplace_back(impl.SimpleName());
        }
      }
      if (!interface_names.empty()) {
        class_implements_[class_name] = interface_names;
      }

      for (auto& m : c->MutableMembers()) {
        if (const auto* fd = dynamic_cast<FieldDecl*>(m.get())) {
          fields.emplace_back(fd->Name(), fd->Type());
        }
        if (const auto* md = dynamic_cast<MethodDecl*>(m.get())) {
          if (md->Name() == class_name) {
            // This is a constructor (method with same name as class)
            MethodSignature sig;
            sig.class_name = class_name;
            sig.method_name = class_name;
            for (const auto& param : md->Params()) {
              sig.param_types.push_back(param.GetType());
            }
            if (md->ReturnType() != nullptr) {
              sig.return_type = std::make_unique<TypeReference>(*md->ReturnType());
            }
            constructors_[class_name] = std::move(sig);
          } else {
            MethodSignature sig;
            sig.class_name = class_name;
            sig.method_name = md->Name();
            for (const auto& param : md->Params()) {
              sig.param_types.push_back(param.GetType());
            }
            if (md->ReturnType() != nullptr) {
              sig.return_type = std::make_unique<TypeReference>(*md->ReturnType());
            }
            methods_[class_name + "::" + md->Name()] = std::move(sig);
          }
        }
        if (const auto* cd = dynamic_cast<CallDecl*>(m.get())) {
          // CallDecl is always a constructor
          MethodSignature sig;
          sig.class_name = class_name;
          sig.method_name = class_name;
          for (const auto& param : cd->Params()) {
            sig.param_types.push_back(param.GetType());
          }
          if (cd->ReturnType() != nullptr) {
            sig.return_type = std::make_unique<TypeReference>(*cd->ReturnType());
          }
          constructors_[class_name] = std::move(sig);
        }
      }
      class_fields_[class_name] = std::move(fields);
    }

    if (auto* ta = dynamic_cast<TypeAliasDecl*>(decl.get())) {
      type_aliases_[ta->Name()] = ta->AliasedType();
    }

    if (auto* gv = dynamic_cast<GlobalVarDecl*>(decl.get())) {
      global_variables_[gv->Name()] = gv->Type();
    }

    if (auto* i = dynamic_cast<InterfaceDecl*>(decl.get())) {
      InterfaceSignature interface_sig;
      interface_sig.interface_name = i->Name();
      for (auto& m : i->MutableMembers()) {
        if (const auto* im = dynamic_cast<InterfaceMethod*>(m.get())) {
          MethodSignature sig;
          sig.class_name = i->Name();
          sig.method_name = im->Name();
          for (const auto& param : im->Params()) {
            sig.param_types.push_back(param.GetType());
          }
          if (im->ReturnType() != nullptr) {
            sig.return_type = std::make_unique<TypeReference>(*im->ReturnType());
          }
          interface_sig.methods[im->Name()] = std::move(sig);
        }
      }
      interfaces_[i->Name()] = std::move(interface_sig);
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(FunctionDecl& node) {
  TypeReference* saved_return_type = current_return_type_;

  if (node.ReturnType() != nullptr) {
    const TypeReference* ret_type = node.ReturnType();
    if (ret_type->SimpleName() == "Void") {
      current_return_type_ = nullptr;
    } else {
      current_return_type_ = node.MutableReturnType();
    }
  } else {
    current_return_type_ = nullptr;
  }

  variable_types_.clear();
  for (const auto& param : node.Params()) {
    variable_types_[param.GetName()] = param.GetType();
  }

  // Register function overload
  FunctionOverload overload;
  for (const auto& param : node.Params()) {
    overload.param_types.push_back(param.GetType());
  }
  if (node.ReturnType() != nullptr) {
    overload.return_type = std::make_unique<TypeReference>(*node.ReturnType());
  }
  function_overloads_[node.Name()].push_back(std::move(overload));

  WalkVisitor::Visit(node);

  current_return_type_ = saved_return_type;
}

void TypeChecker::Visit(ClassDecl& node) {
  std::string saved_class_name = current_class_name_;
  current_class_name_ = node.Name();

  WalkVisitor::Visit(node);

  current_class_name_ = saved_class_name;
}

void TypeChecker::Visit(MethodDecl& node) {
  TypeReference* saved_return_type = current_return_type_;

  if (node.ReturnType() != nullptr) {
    const TypeReference* ret_type = node.ReturnType();
    if (ret_type->SimpleName() == "Void") {
      current_return_type_ = nullptr;
    } else {
      current_return_type_ = node.MutableReturnType();
    }
  } else {
    current_return_type_ = nullptr;
  }

  variable_types_.clear();
  // Add 'this' variable for method bodies
  if (!current_class_name_.empty()) {
    variable_types_["this"] = TypeReference(current_class_name_);
  }
  for (const auto& param : node.Params()) {
    variable_types_[param.GetName()] = param.GetType();
  }

  WalkVisitor::Visit(node);

  current_return_type_ = saved_return_type;
}

void TypeChecker::Visit(VarDeclStmt& node) {
  if (node.MutableInit() != nullptr) {
    TypeReference init_type = InferExpressionType(node.MutableInit());
    TypeReference declared_type = node.MutableType();

    if (!TypesCompatible(declared_type, init_type)) {
      std::ostringstream oss;
      oss << "type mismatch: declared type '" << declared_type.ToStringHuman()
          << "' is not compatible with initializer type '" << init_type.ToStringHuman() << "'";
      sink_.Error("E3003", oss.str(), node.Span());
    }

    node.MutableInit()->Accept(*this);
  }

  variable_types_[node.Name()] = node.Type();
}

void TypeChecker::Visit(GlobalVarDecl& node) {
  if (node.MutableInit() != nullptr) {
    TypeReference init_type = InferExpressionType(node.MutableInit());
    TypeReference declared_type = node.MutableType();

    if (!TypesCompatible(declared_type, init_type)) {
      std::ostringstream oss;
      oss << "type mismatch: declared type '" << declared_type.ToStringHuman()
          << "' is not compatible with initializer type '" << init_type.ToStringHuman() << "'";
      sink_.Error("E3003", oss.str(), node.Span());
    }

    node.MutableInit()->Accept(*this);
  }
}

void TypeChecker::Visit(FieldDecl& node) {
  if (node.MutableInit() != nullptr) {
    TypeReference init_type = InferExpressionType(node.MutableInit());
    const TypeReference& declared_type = node.Type();

    if (!TypesCompatible(declared_type, init_type)) {
      std::ostringstream oss;
      oss << "type mismatch: declared type '" << declared_type.ToStringHuman()
          << "' is not compatible with initializer type '" << init_type.ToStringHuman() << "'";
      sink_.Error("E3003", oss.str(), node.Span());
    }

    node.MutableInit()->Accept(*this);
  }
}

void TypeChecker::Visit(ReturnStmt& node) {
  if (current_return_type_ == nullptr) {
    if (node.HasValue()) {
      sink_.Error("E3001", "return statement in void function cannot have a value", node.Span());
    }
  } else {
    if (!node.HasValue()) {
      sink_.Error("E3002", "return statement must have a value", node.Span());
    } else {
      TypeReference return_type = InferExpressionType(node.MutableValue());
      if (!TypesCompatible(*current_return_type_, return_type)) {
        std::ostringstream oss;
        oss << "return type mismatch: expected '" << current_return_type_->ToStringHuman() << "', got '"
            << return_type.ToStringHuman() << "'";
        sink_.Error("E3004", oss.str(), node.Span());
      }
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(Assign& node) {
  TypeReference target_type = InferExpressionType(&node.MutableTarget());
  TypeReference value_type = InferExpressionType(&node.MutableValue());

  if (!TypesCompatible(target_type, value_type)) {
    std::ostringstream oss;
    oss << "assignment type mismatch: target type '" << target_type.ToStringHuman()
        << "' is not compatible with value type '" << value_type.ToStringHuman() << "'";
    sink_.Error("E3005", oss.str(), node.Span());
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(Call& node) {
  InferExpressionType(&node.MutableCallee());

  // Handle NamespaceRef (system commands like sys::Print)
  if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&node.MutableCallee())) {
    const std::string& func_name = ns_ref->Name();
    // System commands are built-in, don't emit errors for them
    if (kBuiltinSystemCommands.find(func_name) != kBuiltinSystemCommands.end()) {
      // System commands are valid, just visit arguments
      WalkVisitor::Visit(node);
      return;
    }
    // Other namespace calls (like sys::SomeFunction) are also valid
    // Don't emit errors for them
    WalkVisitor::Visit(node);
    return;
  }

  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableCallee())) {
    const std::string& func_name = ident->Name();

    // Check if it's a built-in type name (used as constructor)
    if (kBuiltinTypeNames.find(func_name) != kBuiltinTypeNames.end()) {
      // Built-in type constructors are valid, just visit arguments
      // Don't emit errors for them
      WalkVisitor::Visit(node);
      return;
    }

    // Check if it's a built-in array type constructor
    if (func_name == "IntArray" || func_name == "FloatArray" || func_name == "StringArray" ||
        func_name == "BoolArray" || func_name == "ByteArray" || func_name == "CharArray" ||
        func_name == "ObjectArray") {
      // Built-in array constructors take 2 arguments: size (int) and default value
      if (node.Args().size() != 2) {
        std::ostringstream oss;
        oss << "wrong number of arguments: expected 2, got " << node.Args().size();
        sink_.Error("E3006", oss.str(), node.Span());
      }

      TypeReference arg0_type = InferExpressionType(node.Args()[0].get());
      TypeReference arg1_type = InferExpressionType(node.Args()[1].get());

      if (arg0_type.SimpleName() != "int") {
        std::ostringstream oss;
        oss << "argument 1 type mismatch: expected 'int', got '" << arg0_type.ToStringHuman() << "'";
        sink_.Error("E3007", oss.str(), node.Span());
      }

      if (arg1_type.SimpleName() != GetElementTypeForArray(TypeReference(func_name)).SimpleName()) {
        std::ostringstream oss;
        oss << "argument 2 type mismatch: expected '"
            << GetElementTypeForArray(TypeReference(func_name)).ToStringHuman() << "', got '"
            << arg1_type.ToStringHuman() << "'";
        sink_.Error("E3007", oss.str(), node.Span());
      }
      // Note: We don't check argument types for built-in array constructors
      // as they are handled by the runtime
    } else if (kBuiltinTypeNames.find(func_name) !=
               kBuiltinTypeNames.end()) { // Check if it's a built-in type name (used as constructor)
      // Built-in type constructors are valid, just visit arguments
      // Don't emit errors for them
      WalkVisitor::Visit(node);
      return;
    } else if (class_fields_.find(func_name) != class_fields_.end()) {
      // It's a class constructor - validate arguments if constructor is defined
      if (auto ctor_it = constructors_.find(func_name); ctor_it != constructors_.end()) {
        const auto& sig = ctor_it->second;
        if (node.Args().size() != sig.param_types.size()) {
          std::ostringstream oss;
          oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got " << node.Args().size();
          sink_.Error("E3006", oss.str(), node.Span());
        } else {
          for (size_t i = 0; i < node.Args().size(); ++i) {
            TypeReference arg_type = InferExpressionType(node.Args()[i].get());
            if (!TypesCompatible(sig.param_types[i], arg_type)) {
              std::ostringstream oss;
              oss << "argument " << (i + 1) << " type mismatch: expected '" << sig.param_types[i].ToStringHuman()
                  << "', got '" << arg_type.ToStringHuman() << "'";
              sink_.Error("E3007", oss.str(), node.Span());
            }
          }
        }
      }
      // If no constructor found, allow any number of arguments for now
      // (language may support default/implicit constructors)
    } else {
      // Check if function exists
      auto overloads_it = function_overloads_.find(func_name);
      if (overloads_it == function_overloads_.end() || overloads_it->second.empty()) {
        // Function doesn't exist
        std::ostringstream oss;
        oss << "no matching overload for function '" << func_name << "' with " << node.Args().size() << " argument(s)";
        sink_.Error("E3008", oss.str(), node.Span());
        WalkVisitor::Visit(node);
        return;
      }

      // First check argument count - find any overload with matching count
      bool found_matching_count = false;
      for (const auto& overload : overloads_it->second) {
        if (overload.param_types.size() == node.Args().size()) {
          found_matching_count = true;
          break;
        }
      }

      if (!found_matching_count) {
        // Wrong number of arguments
        std::ostringstream oss;
        oss << "wrong number of arguments: expected " << overloads_it->second[0].param_types.size() << ", got "
            << node.Args().size();
        sink_.Error("E3006", oss.str(), node.Span());
        WalkVisitor::Visit(node);
        return;
      }

      // Try to resolve function overload
      const FunctionOverload* resolved = ResolveFunctionOverload(func_name, node.Args());
      if (resolved != nullptr) {
        // Validate argument types
        for (size_t i = 0; i < node.Args().size(); ++i) {
          TypeReference arg_type = InferExpressionType(node.Args()[i].get());
          if (!TypesCompatible(resolved->param_types[i], arg_type)) {
            std::ostringstream oss;
            oss << "argument " << (i + 1) << " type mismatch: expected '" << resolved->param_types[i].ToStringHuman()
                << "', got '" << arg_type.ToStringHuman() << "'";
            sink_.Error("E3007", oss.str(), node.Span());
          }
        }
      } else {
        // Argument count matches but types don't - check each argument
        // Find the first overload with matching argument count
        const FunctionOverload* candidate = nullptr;
        for (const auto& overload : overloads_it->second) {
          if (overload.param_types.size() == node.Args().size()) {
            candidate = &overload;
            break;
          }
        }

        if (candidate != nullptr) {
          // Check each argument type
          for (size_t i = 0; i < node.Args().size(); ++i) {
            TypeReference arg_type = InferExpressionType(node.Args()[i].get());
            if (!TypesCompatible(candidate->param_types[i], arg_type)) {
              std::ostringstream oss;
              oss << "argument " << (i + 1) << " type mismatch: expected '" << candidate->param_types[i].ToStringHuman()
                  << "', got '" << arg_type.ToStringHuman() << "'";
              sink_.Error("E3007", oss.str(), node.Span());
            }
          }
        } else {
          // No matching overload found (wrong types)
          std::ostringstream oss;
          oss << "no matching overload for function '" << func_name << "' with " << node.Args().size()
              << " argument(s)";
          sink_.Error("E3008", oss.str(), node.Span());
        }
      }
    }
  } else if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableCallee())) {
    TypeReference object_type = InferExpressionType(&field_access->MutableObject());
    std::string class_name;
    if (!object_type.QualifiedName().empty()) {
      class_name = std::string(object_type.SimpleName());
    }
    if (!class_name.empty()) {
      // Check builtin methods first
      const BuiltinMethodSignature* builtin_sig = FindBuiltinMethod(class_name, field_access->Name());
      if (builtin_sig != nullptr) {
        if (node.Args().size() != builtin_sig->param_types.size()) {
          std::ostringstream oss;
          oss << "wrong number of arguments: expected " << builtin_sig->param_types.size() << ", got "
              << node.Args().size();
          sink_.Error("E3006", oss.str(), node.Span());
        } else {
          for (size_t i = 0; i < node.Args().size(); ++i) {
            TypeReference arg_type = InferExpressionType(node.Args()[i].get());
            if (!TypesCompatible(builtin_sig->param_types[i], arg_type)) {
              std::ostringstream oss;
              oss << "argument " << (i + 1) << " type mismatch: expected '"
                  << builtin_sig->param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
              sink_.Error("E3007", oss.str(), node.Span());
            }
          }
        }
      } else {
        std::string method_key = class_name + "::" + field_access->Name();
        if (auto method_it = methods_.find(method_key); method_it != methods_.end()) {
          const auto& sig = method_it->second;
          if (node.Args().size() != sig.param_types.size()) {
            std::ostringstream oss;
            oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got " << node.Args().size();
            sink_.Error("E3006", oss.str(), node.Span());
          } else {
            for (size_t i = 0; i < node.Args().size(); ++i) {
              TypeReference arg_type = InferExpressionType(node.Args()[i].get());
              if (!TypesCompatible(sig.param_types[i], arg_type)) {
                std::ostringstream oss;
                oss << "argument " << (i + 1) << " type mismatch: expected '" << sig.param_types[i].ToStringHuman()
                    << "', got '" << arg_type.ToStringHuman() << "'";
                sink_.Error("E3007", oss.str(), node.Span());
              }
            }
          }
        } else if (auto interface_it = interfaces_.find(class_name); interface_it != interfaces_.end()) {
          // Check if class_name is actually an interface
          const auto& interface_sig = interface_it->second;
          if (auto interface_method_it = interface_sig.methods.find(field_access->Name());
              interface_method_it != interface_sig.methods.end()) {
            const auto& sig = interface_method_it->second;
            if (node.Args().size() != sig.param_types.size()) {
              std::ostringstream oss;
              oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got " << node.Args().size();
              sink_.Error("E3006", oss.str(), node.Span());
            } else {
              for (size_t i = 0; i < node.Args().size(); ++i) {
                TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                if (!TypesCompatible(sig.param_types[i], arg_type)) {
                  std::ostringstream oss;
                  oss << "argument " << (i + 1) << " type mismatch: expected '" << sig.param_types[i].ToStringHuman()
                      << "', got '" << arg_type.ToStringHuman() << "'";
                  sink_.Error("E3007", oss.str(), node.Span());
                }
              }
            }
          } else {
            std::ostringstream oss;
            oss << "unknown method '" << field_access->Name() << "' in interface '" << class_name << "'";
            sink_.Error("E3014", oss.str(), node.Span());
          }
        } else {
          // Check Object interface for class types
          bool found_in_interface = false;
          if (IsClassType(class_name)) {
            if (auto object_it = interfaces_.find("Object"); object_it != interfaces_.end()) {
              const auto& object_sig = object_it->second;
              if (auto object_method_it = object_sig.methods.find(field_access->Name());
                  object_method_it != object_sig.methods.end()) {
                const auto& sig = object_method_it->second;
                if (node.Args().size() != sig.param_types.size()) {
                  std::ostringstream oss;
                  oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got "
                      << node.Args().size();
                  sink_.Error("E3006", oss.str(), node.Span());
                } else {
                  for (size_t i = 0; i < node.Args().size(); ++i) {
                    TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                    if (!TypesCompatible(sig.param_types[i], arg_type)) {
                      std::ostringstream oss;
                      oss << "argument " << (i + 1) << " type mismatch: expected '"
                          << sig.param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
                      sink_.Error("E3007", oss.str(), node.Span());
                    }
                  }
                }
                found_in_interface = true;
              }
            }
          }
          // Check explicitly implemented interfaces (including IComparable, IHashable, IStringConvertible)
          if (!found_in_interface && class_implements_.find(class_name) != class_implements_.end()) {
            const auto& impls = class_implements_[class_name];
            for (const auto& impl_name : impls) {
              if (auto interface_it = interfaces_.find(impl_name); interface_it != interfaces_.end()) {
                const auto& interface_sig = interface_it->second;
                if (auto interface_method_it = interface_sig.methods.find(field_access->Name());
                    interface_method_it != interface_sig.methods.end()) {
                  const auto& sig = interface_method_it->second;
                  if (node.Args().size() != sig.param_types.size()) {
                    std::ostringstream oss;
                    oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got "
                        << node.Args().size();
                    sink_.Error("E3006", oss.str(), node.Span());
                  } else {
                    for (size_t i = 0; i < node.Args().size(); ++i) {
                      TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                      if (!TypesCompatible(sig.param_types[i], arg_type)) {
                        std::ostringstream oss;
                        oss << "argument " << (i + 1) << " type mismatch: expected '"
                            << sig.param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
                        sink_.Error("E3007", oss.str(), node.Span());
                      }
                    }
                  }
                  found_in_interface = true;
                  break;
                }
              }
            }
          }
          if (!found_in_interface) {
            std::ostringstream oss;
            oss << "unknown method '" << field_access->Name() << "' in class '" << class_name << "'";
            sink_.Error("E3014", oss.str(), node.Span());
          }
        }
      }
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(FieldAccess& node) {
  TypeReference object_type = InferExpressionType(&node.MutableObject());

  std::string class_name;
  if (!object_type.QualifiedName().empty()) {
    class_name = std::string(object_type.SimpleName());
  }

  if (!class_name.empty()) {
    // Check builtin methods first
    const BuiltinMethodSignature* builtin_sig = FindBuiltinMethod(class_name, node.Name());
    if (builtin_sig != nullptr) {
      // Builtin method found, no error
      WalkVisitor::Visit(node);
      return;
    }

    if (auto fields_it = class_fields_.find(class_name); fields_it != class_fields_.end()) {
      const auto& fields = fields_it->second;
      bool found = false;
      for (const auto& field_name : fields | std::views::keys) {
        if (field_name == node.Name()) {
          found = true;
          break;
        }
      }
      // Also check if it's a method
      if (!found) {
        std::string method_key = class_name + "::" + node.Name();
        if (methods_.find(method_key) == methods_.end()) {
          // Also check interfaces (including implicit Object interface for class types)
          bool found_in_interface = false;
          if (auto interface_it = interfaces_.find(class_name); interface_it != interfaces_.end()) {
            const auto& interface_sig = interface_it->second;
            if (interface_sig.methods.find(node.Name()) != interface_sig.methods.end()) {
              found_in_interface = true;
            }
          }
          // Check if class implements builtin interfaces (all class types implicitly do)
          if (!found_in_interface && IsClassType(class_name)) {
            // Check Object interface
            if (auto object_it = interfaces_.find("Object"); object_it != interfaces_.end()) {
              const auto& object_sig = object_it->second;
              if (object_sig.methods.find(node.Name()) != object_sig.methods.end()) {
                found_in_interface = true;
              }
            }
            // Check IComparable interface
            if (!found_in_interface) {
              if (auto comparable_it = interfaces_.find("IComparable"); comparable_it != interfaces_.end()) {
                const auto& comparable_sig = comparable_it->second;
                if (comparable_sig.methods.find(node.Name()) != comparable_sig.methods.end()) {
                  found_in_interface = true;
                }
              }
            }
            // Check IHashable interface
            if (!found_in_interface) {
              if (auto hashable_it = interfaces_.find("IHashable"); hashable_it != interfaces_.end()) {
                const auto& hashable_sig = hashable_it->second;
                if (hashable_sig.methods.find(node.Name()) != hashable_sig.methods.end()) {
                  found_in_interface = true;
                }
              }
            }
            // Check IStringConvertible interface
            if (!found_in_interface) {
              if (auto string_convertible_it = interfaces_.find("IStringConvertible");
                  string_convertible_it != interfaces_.end()) {
                const auto& string_convertible_sig = string_convertible_it->second;
                if (string_convertible_sig.methods.find(node.Name()) != string_convertible_sig.methods.end()) {
                  found_in_interface = true;
                }
              }
            }
          }
          // Check if class explicitly implements other interfaces
          if (!found_in_interface && class_implements_.find(class_name) != class_implements_.end()) {
            const auto& impls = class_implements_[class_name];
            for (const auto& impl_name : impls) {
              if (auto interface_it = interfaces_.find(impl_name); interface_it != interfaces_.end()) {
                const auto& interface_sig = interface_it->second;
                if (interface_sig.methods.find(node.Name()) != interface_sig.methods.end()) {
                  found_in_interface = true;
                  break;
                }
              }
            }
          }
          if (!found_in_interface) {
            std::ostringstream oss;
            oss << "field '" << node.Name() << "' not found in class '" << class_name << "'";
            sink_.Error("E3009", oss.str(), node.Span());
          }
        }
      }
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(IndexAccess& node) {
  InferExpressionType(&node.MutableObject());
  TypeReference index_type = InferExpressionType(&node.MutableIndexExpr());

  // Check if index type is int (must be exact match)
  std::string index_simple;
  if (!index_type.QualifiedName().empty()) {
    index_simple = std::string(index_type.SimpleName());
  }
  std::string index_fundamental = GetFundamentalTypeName(index_type);

  // Check if it's exactly "int" or if fundamental type is "int"
  if (index_simple != "int" && index_fundamental != "int") {
    std::ostringstream oss;
    oss << "array index must be of type 'int', got '" << index_type.ToStringHuman() << "'";
    sink_.Error("E3010", oss.str(), node.Span());
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(Binary& node) {
  TypeReference lhs_type = InferExpressionType(&node.MutableLhs());
  TypeReference rhs_type = InferExpressionType(&node.MutableRhs());

  const IBinaryOpTag& op = node.Op();
  bool is_comparison = &op == &optags::Eq() || &op == &optags::Ne() || &op == &optags::Lt() || &op == &optags::Le() ||
                       &op == &optags::Gt() || &op == &optags::Ge();
  bool is_logical = &op == &optags::And() || &op == &optags::Or();

  if (is_logical) {
    // Logical operators require bool operands
    TypeReference bool_type("bool");
    if (!TypesCompatible(bool_type, lhs_type) || !TypesCompatible(bool_type, rhs_type)) {
      std::ostringstream oss;
      oss << "operands of '" << std::string(op.Name()) << "' must be of type 'bool'";
      sink_.Error("E3011", oss.str(), node.Span());
    }
  } else if (is_comparison) {
    // Comparison operators require operands of the same fundamental type
    // Result is bool
    std::string lhs_fundamental = GetFundamentalTypeName(lhs_type);
    std::string rhs_fundamental = GetFundamentalTypeName(rhs_type);

    if (lhs_fundamental.empty() || rhs_fundamental.empty() || lhs_fundamental != rhs_fundamental) {
      std::ostringstream oss;
      oss << "operands of '" << std::string(op.Name()) << "' must have the same fundamental type: '"
          << lhs_type.ToStringHuman() << "' and '" << rhs_type.ToStringHuman() << "'";
      sink_.Error("E3011", oss.str(), node.Span());
    }
  } else if (!TypesCompatible(lhs_type, rhs_type)) {
    std::ostringstream oss;
    oss << "operands of '" << std::string(op.Name()) << "' must have compatible types: '" << lhs_type.ToStringHuman()
        << "' and '" << rhs_type.ToStringHuman() << "'";
    sink_.Error("E3012", oss.str(), node.Span());
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(Unary& node) {
  TypeReference operand_type = InferExpressionType(&node.MutableOperand());

  const IUnaryOpTag& op = node.Op();
  if (&op == &optags::Not()) {
    TypeReference bool_type("bool");
    if (!TypesCompatible(bool_type, operand_type)) {
      sink_.Error("E3013", "operand of 'not' must be of type 'bool'", node.Span());
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(SafeCall& node) {
  TypeReference object_type = InferExpressionType(&node.MutableObject());

  std::string class_name;
  if (!object_type.QualifiedName().empty()) {
    class_name = std::string(object_type.SimpleName());
  }

  if (!class_name.empty()) {
    // Remove nullable to check for methods
    TypeReference non_nullable_type = object_type.IsNullable() ? object_type.WithoutNullable() : object_type;
    std::string non_nullable_class_name;
    if (!non_nullable_type.QualifiedName().empty()) {
      non_nullable_class_name = std::string(non_nullable_type.SimpleName());
    }

    if (!non_nullable_class_name.empty()) {
      // Check builtin methods first
      const BuiltinMethodSignature* builtin_sig = FindBuiltinMethod(non_nullable_class_name, node.Method());
      if (builtin_sig != nullptr) {
        if (node.Args().size() != builtin_sig->param_types.size()) {
          std::ostringstream oss;
          oss << "wrong number of arguments: expected " << builtin_sig->param_types.size() << ", got "
              << node.Args().size();
          sink_.Error("E3006", oss.str(), node.Span());
        } else {
          for (size_t i = 0; i < node.Args().size(); ++i) {
            TypeReference arg_type = InferExpressionType(node.Args()[i].get());
            if (!TypesCompatible(builtin_sig->param_types[i], arg_type)) {
              std::ostringstream oss;
              oss << "argument " << (i + 1) << " type mismatch: expected '"
                  << builtin_sig->param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
              sink_.Error("E3007", oss.str(), node.Span());
            }
          }
        }
      } else {
        std::string method_key = non_nullable_class_name + "::" + node.Method();
        if (auto method_it = methods_.find(method_key); method_it != methods_.end()) {
          const auto& sig = method_it->second;
          if (node.Args().size() != sig.param_types.size()) {
            std::ostringstream oss;
            oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got " << node.Args().size();
            sink_.Error("E3006", oss.str(), node.Span());
          } else {
            for (size_t i = 0; i < node.Args().size(); ++i) {
              TypeReference arg_type = InferExpressionType(node.Args()[i].get());
              if (!TypesCompatible(sig.param_types[i], arg_type)) {
                std::ostringstream oss;
                oss << "argument " << (i + 1) << " type mismatch: expected '" << sig.param_types[i].ToStringHuman()
                    << "', got '" << arg_type.ToStringHuman() << "'";
                sink_.Error("E3007", oss.str(), node.Span());
              }
            }
          }
        } else if (auto interface_it = interfaces_.find(non_nullable_class_name); interface_it != interfaces_.end()) {
          // Check if non_nullable_class_name is actually an interface
          const auto& interface_sig = interface_it->second;
          if (auto interface_method_it = interface_sig.methods.find(node.Method());
              interface_method_it != interface_sig.methods.end()) {
            const auto& sig = interface_method_it->second;
            if (node.Args().size() != sig.param_types.size()) {
              std::ostringstream oss;
              oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got " << node.Args().size();
              sink_.Error("E3006", oss.str(), node.Span());
            } else {
              for (size_t i = 0; i < node.Args().size(); ++i) {
                TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                if (!TypesCompatible(sig.param_types[i], arg_type)) {
                  std::ostringstream oss;
                  oss << "argument " << (i + 1) << " type mismatch: expected '" << sig.param_types[i].ToStringHuman()
                      << "', got '" << arg_type.ToStringHuman() << "'";
                  sink_.Error("E3007", oss.str(), node.Span());
                }
              }
            }
          } else {
            std::ostringstream oss;
            oss << "unknown method '" << node.Method() << "' in interface '" << non_nullable_class_name << "'";
            sink_.Error("E3014", oss.str(), node.Span());
          }
        } else {
          // Check Object interface for class types
          bool found_in_interface = false;
          if (IsClassType(non_nullable_class_name)) {
            if (auto object_it = interfaces_.find("Object"); object_it != interfaces_.end()) {
              const auto& object_sig = object_it->second;
              if (auto object_method_it = object_sig.methods.find(node.Method());
                  object_method_it != object_sig.methods.end()) {
                const auto& sig = object_method_it->second;
                if (node.Args().size() != sig.param_types.size()) {
                  std::ostringstream oss;
                  oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got "
                      << node.Args().size();
                  sink_.Error("E3006", oss.str(), node.Span());
                } else {
                  for (size_t i = 0; i < node.Args().size(); ++i) {
                    TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                    if (!TypesCompatible(sig.param_types[i], arg_type)) {
                      std::ostringstream oss;
                      oss << "argument " << (i + 1) << " type mismatch: expected '"
                          << sig.param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
                      sink_.Error("E3007", oss.str(), node.Span());
                    }
                  }
                }
                found_in_interface = true;
              }
            }
          }
          // Check explicitly implemented interfaces (including IComparable, IHashable, IStringConvertible)
          if (!found_in_interface && class_implements_.find(non_nullable_class_name) != class_implements_.end()) {
            const auto& impls = class_implements_[non_nullable_class_name];
            for (const auto& impl_name : impls) {
              if (auto interface_it = interfaces_.find(impl_name); interface_it != interfaces_.end()) {
                const auto& interface_sig = interface_it->second;
                if (auto interface_method_it = interface_sig.methods.find(node.Method());
                    interface_method_it != interface_sig.methods.end()) {
                  const auto& sig = interface_method_it->second;
                  if (node.Args().size() != sig.param_types.size()) {
                    std::ostringstream oss;
                    oss << "wrong number of arguments: expected " << sig.param_types.size() << ", got "
                        << node.Args().size();
                    sink_.Error("E3006", oss.str(), node.Span());
                  } else {
                    for (size_t i = 0; i < node.Args().size(); ++i) {
                      TypeReference arg_type = InferExpressionType(node.Args()[i].get());
                      if (!TypesCompatible(sig.param_types[i], arg_type)) {
                        std::ostringstream oss;
                        oss << "argument " << (i + 1) << " type mismatch: expected '"
                            << sig.param_types[i].ToStringHuman() << "', got '" << arg_type.ToStringHuman() << "'";
                        sink_.Error("E3007", oss.str(), node.Span());
                      }
                    }
                  }
                  found_in_interface = true;
                  break;
                }
              }
            }
          }
          if (!found_in_interface && node.Args().empty()) {
            // SafeCall with no arguments might be field access (p?.x)
            // Check if it's a field
            if (auto fields_it = class_fields_.find(non_nullable_class_name); fields_it != class_fields_.end()) {
              bool found_field = false;
              for (const auto& field_name : fields_it->second | std::views::keys) {
                if (field_name == node.Method()) {
                  found_field = true;
                  break;
                }
              }
              if (!found_field) {
                std::ostringstream oss;
                oss << "unknown method '" << node.Method() << "' in class '" << non_nullable_class_name << "'";
                sink_.Error("E3014", oss.str(), node.Span());
              }
            } else {
              std::ostringstream oss;
              oss << "unknown method '" << node.Method() << "' in class '" << non_nullable_class_name << "'";
              sink_.Error("E3014", oss.str(), node.Span());
            }
          } else {
            std::ostringstream oss;
            oss << "unknown method '" << node.Method() << "' in class '" << non_nullable_class_name << "'";
            sink_.Error("E3014", oss.str(), node.Span());
          }
        }
      }
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(Elvis& node) {
  TypeReference lhs_type = InferExpressionType(&node.MutableLhs());
  TypeReference rhs_type = InferExpressionType(&node.MutableRhs());

  // Validate that lhs and rhs types are compatible for Elvis operator
  if (lhs_type.IsNullable()) {
    TypeReference lhs_non_nullable = lhs_type.WithoutNullable();
    // For Elvis operator, rhs should be compatible with non-nullable lhs type
    if (!TypesCompatible(lhs_non_nullable, rhs_type) && !TypesCompatible(rhs_type, lhs_non_nullable)) {
      std::ostringstream oss;
      oss << "Elvis operator requires compatible types: lhs type '" << lhs_non_nullable.ToStringHuman()
          << "' and rhs type '" << rhs_type.ToStringHuman() << "' are incompatible";
      sink_.Error("E3012", oss.str(), node.Span());
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(CastAs& node) {
  InferExpressionType(&node.MutableExpression());

  // Basic validation: source and target types should be related
  // For now, we allow any cast (runtime will handle invalid casts)
  // But we check that the types are reference types (not primitives)

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(TypeTestIs& node) {
  // Type test always returns bool, no validation needed here
  // Runtime will handle the actual type checking

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(InterfaceDecl& node) {
  // Interfaces are registered in Visit(Module&), nothing to do here
  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(IdentRef& node) {
  // Don't emit errors for built-in type names - they're valid identifiers
  if (kBuiltinTypeNames.find(node.Name()) != kBuiltinTypeNames.end()) {
    WalkVisitor::Visit(node);
    return;
  }

  // Don't emit errors for system commands - they're valid identifiers
  if (kBuiltinSystemCommands.find(node.Name()) != kBuiltinSystemCommands.end()) {
    WalkVisitor::Visit(node);
    return;
  }

  if (variable_types_.find(node.Name()) == variable_types_.end() &&
      global_variables_.find(node.Name()) == global_variables_.end() &&
      functions_.find(node.Name()) == functions_.end() && class_fields_.find(node.Name()) == class_fields_.end()) {
    // Don't emit error for unknown identifiers - they might be valid at runtime
    // or handled by the bytecode visitor (e.g., system functions, etc.)
  }
  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(ExprStmt& node) {
  WalkVisitor::Visit(node);
}

TypeReference* TypeChecker::ResolveTypeAlias(const TypeReference& type) {
  if (type.QualifiedName().empty()) {
    return nullptr;
  }

  std::string type_name = std::string(type.SimpleName());
  if (auto alias_it = type_aliases_.find(type_name); alias_it != type_aliases_.end()) {
    return &alias_it->second;
  }

  return nullptr;
}

TypeReference TypeChecker::InferExpressionType(Expr* expr) {
  if (expr == nullptr) {
    return {};
  }

  // Check IdentRef first (like BytecodeVisitor does)
  if (auto* ident = dynamic_cast<IdentRef*>(expr)) {
    if (auto var_it = variable_types_.find(ident->Name()); var_it != variable_types_.end()) {
      return var_it->second;
    }
    if (auto global_it = global_variables_.find(ident->Name()); global_it != global_variables_.end()) {
      return global_it->second;
    }
    return {};
  }

  // Check FieldAccess
  if (auto* field_access = dynamic_cast<FieldAccess*>(expr)) {
    TypeReference object_type = InferExpressionType(&field_access->MutableObject());
    std::string class_name;
    if (!object_type.QualifiedName().empty()) {
      class_name = std::string(object_type.SimpleName());
    }
    if (!class_name.empty()) {
      if (auto fields_it = class_fields_.find(class_name); fields_it != class_fields_.end()) {
        for (const auto& [field_name, field_type] : fields_it->second) {
          if (field_name == field_access->Name()) {
            return field_type;
          }
        }
      }
    }
    return {};
  }

  // Check literals (after IdentRef and FieldAccess, like BytecodeVisitor)
  if (dynamic_cast<IntLit*>(expr) != nullptr) {
    return TypeReference("int");
  }
  if (dynamic_cast<FloatLit*>(expr) != nullptr) {
    return TypeReference("float");
  }
  if (dynamic_cast<StringLit*>(expr) != nullptr) {
    return TypeReference("String");
  }
  if (dynamic_cast<BoolLit*>(expr) != nullptr) {
    return TypeReference("bool");
  }
  if (dynamic_cast<CharLit*>(expr) != nullptr) {
    return TypeReference("char");
  }
  if (dynamic_cast<ByteLit*>(expr) != nullptr) {
    return TypeReference("byte");
  }
  if (dynamic_cast<NullLit*>(expr) != nullptr) {
    TypeReference null_type("Object");
    null_type.MakeNullable();
    return null_type;
  }

  if (dynamic_cast<ThisExpr*>(expr)) {
    if (!current_class_name_.empty()) {
      return TypeReference(current_class_name_);
    }
    return {};
  }

  if (auto* call = dynamic_cast<Call*>(expr)) {
    // Handle NamespaceRef (sys:: functions)
    if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&call->MutableCallee())) {
      std::string ns_name = ns_ref->Name();
      // Check if namespace is "sys" by examining NamespaceExpr
      if (const auto* ns_ident = dynamic_cast<IdentRef*>(&ns_ref->MutableNamespaceExpr())) {
        if (ns_ident->Name() == "sys") {
          // Handle GetEnvironmentVar - returns Nullable<String>
          if (ns_name == "GetEnvironmentVar" && call->Args().size() == 1) {
            TypeReference result("String");
            result.MakeNullable();
            return result;
          }
          // Check for built-in return types
          if (const auto it = kBuiltinReturnTypes.find(ns_name); it != kBuiltinReturnTypes.end()) {
            std::string return_type_name = it->second;
            // Handle Void return type
            if (return_type_name == "Void") {
              return {}; // Empty TypeReference for void
            }
            return TypeReference(return_type_name);
          }
          // Handle special cases with argument checks
          if (ns_name == "Sqrt" && call->Args().size() == 1) {
            return TypeReference("float");
          }
          if (ns_name == "ToString" && call->Args().size() == 1) {
            return TypeReference("String");
          }
          if (ns_name == "ToInt" && call->Args().size() == 1) {
            return TypeReference("int");
          }
          if (ns_name == "ToFloat" && call->Args().size() == 1) {
            return TypeReference("float");
          }
          // Default: system commands that don't return values return Void (empty)
          return {};
        }
      }
      // For non-sys namespace calls, return empty (unknown)
      return {};
    }

    if (auto* ident = dynamic_cast<IdentRef*>(&call->MutableCallee())) {
      const std::string& func_name = ident->Name();

      // Check if it's a built-in type name (used as constructor)
      if (kBuiltinTypeNames.find(func_name) != kBuiltinTypeNames.end()) {
        // Return the type name (e.g., Int, Float, String, etc.)
        return TypeReference(func_name);
      }

      // Check if it's a built-in array type constructor
      if (func_name == "IntArray") {
        return TypeReference("IntArray");
      }
      if (func_name == "FloatArray") {
        return TypeReference("FloatArray");
      }
      if (func_name == "StringArray") {
        return TypeReference("StringArray");
      }
      if (func_name == "BoolArray") {
        return TypeReference("BoolArray");
      }
      if (func_name == "ByteArray") {
        return TypeReference("ByteArray");
      }
      if (func_name == "CharArray") {
        return TypeReference("CharArray");
      }
      if (func_name == "ObjectArray") {
        return TypeReference("ObjectArray");
      }

      if (auto func_it = functions_.find(func_name); func_it != functions_.end()) {
        if (func_it->second.return_type) {
          return *func_it->second.return_type;
        }
      }

      // Check if it's a class constructor
      if (class_fields_.find(func_name) != class_fields_.end()) {
        return TypeReference(func_name);
      }
    } else if (auto* field_access = dynamic_cast<FieldAccess*>(&call->MutableCallee())) {
      TypeReference object_type = InferExpressionType(&field_access->MutableObject());
      std::string class_name;
      if (!object_type.QualifiedName().empty()) {
        class_name = std::string(object_type.SimpleName());
      }
      if (!class_name.empty()) {
        // Check builtin methods first
        const BuiltinMethodSignature* builtin_sig = FindBuiltinMethod(class_name, field_access->Name());
        if (builtin_sig != nullptr && builtin_sig->return_type != nullptr) {
          return *builtin_sig->return_type;
        }

        // Then check user-defined methods
        std::string method_key = class_name + "::" + field_access->Name();
        if (auto method_it = methods_.find(method_key); method_it != methods_.end()) {
          if (method_it->second.return_type) {
            return *method_it->second.return_type;
          }
        } else if (auto interface_it = interfaces_.find(class_name); interface_it != interfaces_.end()) {
          const auto& interface_sig = interface_it->second;
          if (auto interface_method_it = interface_sig.methods.find(field_access->Name());
              interface_method_it != interface_sig.methods.end()) {
            if (interface_method_it->second.return_type) {
              return *interface_method_it->second.return_type;
            }
          }
        }
        // Check Object interface for class types
        if (IsClassType(class_name)) {
          if (auto object_it = interfaces_.find("Object"); object_it != interfaces_.end()) {
            const auto& object_sig = object_it->second;
            if (auto object_method_it = object_sig.methods.find(field_access->Name());
                object_method_it != object_sig.methods.end()) {
              if (object_method_it->second.return_type) {
                return *object_method_it->second.return_type;
              }
            }
          }
        }
        // Check explicitly implemented interfaces (including IComparable, IHashable, IStringConvertible)
        if (class_implements_.find(class_name) != class_implements_.end()) {
          const auto& impls = class_implements_[class_name];
          for (const auto& impl_name : impls) {
            if (auto interface_it = interfaces_.find(impl_name); interface_it != interfaces_.end()) {
              const auto& interface_sig = interface_it->second;
              if (auto interface_method_it = interface_sig.methods.find(field_access->Name());
                  interface_method_it != interface_sig.methods.end()) {
                if (interface_method_it->second.return_type) {
                  return *interface_method_it->second.return_type;
                }
              }
            }
          }
        }
      }
    }
    return {};
  }

  if (auto* index_access = dynamic_cast<IndexAccess*>(expr)) {
    TypeReference object_type = InferExpressionType(&index_access->MutableObject());

    // If we couldn't get the type from InferExpressionType, try getting it directly from IdentRef
    if (object_type.QualifiedName().empty()) {
      if (auto* ident = dynamic_cast<IdentRef*>(&index_access->MutableObject())) {
        if (auto var_it = variable_types_.find(ident->Name()); var_it != variable_types_.end()) {
          object_type = var_it->second;
        } else if (auto global_it = global_variables_.find(ident->Name()); global_it != global_variables_.end()) {
          object_type = global_it->second;
        }
      }
    }

    // Get element type for array types
    return GetElementTypeForArray(object_type);
  }

  if (auto* safe_call = dynamic_cast<SafeCall*>(expr)) {
    TypeReference object_type = InferExpressionType(&safe_call->MutableObject());
    std::string class_name;
    if (!object_type.QualifiedName().empty()) {
      class_name = std::string(object_type.SimpleName());
    }
    if (!class_name.empty()) {
      // Remove nullable to check for methods
      TypeReference non_nullable_type = object_type.IsNullable() ? object_type.WithoutNullable() : object_type;
      std::string non_nullable_class_name;
      if (!non_nullable_type.QualifiedName().empty()) {
        non_nullable_class_name = std::string(non_nullable_type.SimpleName());
      }

      if (!non_nullable_class_name.empty()) {
        std::string method_key = non_nullable_class_name + "::" + safe_call->Method();
        if (auto method_it = methods_.find(method_key); method_it != methods_.end()) {
          if (method_it->second.return_type) {
            TypeReference return_type = *method_it->second.return_type;
            // Safe call always returns nullable type (since object can be null)
            return_type.MakeNullable();
            return return_type;
          }
        } else if (auto interface_it = interfaces_.find(non_nullable_class_name); interface_it != interfaces_.end()) {
          // Check if non_nullable_class_name is actually an interface
          const auto& interface_sig = interface_it->second;
          if (auto interface_method_it = interface_sig.methods.find(safe_call->Method());
              interface_method_it != interface_sig.methods.end()) {
            if (interface_method_it->second.return_type) {
              TypeReference return_type = *interface_method_it->second.return_type;
              // Safe call always returns nullable type (since object can be null)
              return_type.MakeNullable();
              return return_type;
            }
          }
        } else if (safe_call->Args().empty()) {
          // SafeCall with no arguments might be field access (p?.x)
          // Check if it's a field
          if (auto fields_it = class_fields_.find(non_nullable_class_name); fields_it != class_fields_.end()) {
            for (const auto& [field_name, field_type] : fields_it->second) {
              if (field_name == safe_call->Method()) {
                // Safe call always returns nullable type (since object can be null)
                TypeReference return_type = field_type;
                return_type.MakeNullable();
                return return_type;
              }
            }
          }
        }
      }
    }
    return {};
  }

  if (auto* binary = dynamic_cast<Binary*>(expr)) {
    TypeReference lhs_type = InferExpressionType(&binary->MutableLhs());
    TypeReference rhs_type = InferExpressionType(&binary->MutableRhs());

    const IBinaryOpTag& op = binary->Op();
    bool is_comparison = &op == &optags::Eq() || &op == &optags::Ne() || &op == &optags::Lt() || &op == &optags::Le() ||
                         &op == &optags::Gt() || &op == &optags::Ge();
    bool is_logical = &op == &optags::And() || &op == &optags::Or();

    if (is_comparison || is_logical) {
      return TypeReference("bool");
    } else {
      // Arithmetic operations return the type of the operands
      // Use type promotion: if either operand is float, result is float
      // Otherwise use the type of the first non-empty operand
      std::string lhs_fundamental = GetFundamentalTypeName(lhs_type);
      std::string rhs_fundamental = GetFundamentalTypeName(rhs_type);

      // Type promotion: float > int > byte/char
      if (lhs_fundamental == "float" || rhs_fundamental == "float") {
        return TypeReference("float");
      }
      if (lhs_fundamental == "int" || rhs_fundamental == "int") {
        return TypeReference("int");
      }
      if (!lhs_fundamental.empty()) {
        return TypeReference(lhs_fundamental);
      }
      if (!rhs_fundamental.empty()) {
        return TypeReference(rhs_fundamental);
      }
      // If both are empty, return empty (shouldn't happen for valid expressions)
      return {};
    }
  }

  if (auto* unary = dynamic_cast<Unary*>(expr)) {
    TypeReference operand_type = InferExpressionType(&unary->MutableOperand());

    const IUnaryOpTag& op = unary->Op();
    if (&op == &optags::Not()) {
      return TypeReference("bool");
    } else {
      // Other unary operations (negation, plus) return the type of the operand
      return operand_type;
    }
  }

  if (auto* elvis = dynamic_cast<Elvis*>(expr)) {
    TypeReference lhs_type = InferExpressionType(&elvis->MutableLhs());
    TypeReference rhs_type = InferExpressionType(&elvis->MutableRhs());

    // Elvis operator: if lhs is null, use rhs
    // Result type is the non-nullable version of lhs type, or rhs type if lhs is nullable
    // The result is always non-nullable (since rhs is used if lhs is null)
    if (lhs_type.IsNullable()) {
      // Result is the union: non-nullable lhs type, or rhs type (whichever is more general)
      // For simplicity, return rhs type (which should be non-nullable)
      return rhs_type;
    } else {
      // If lhs is not nullable, result is lhs type
      return lhs_type;
    }
  }

  if (auto* cast = dynamic_cast<CastAs*>(expr)) {
    // Cast returns the target type
    return cast->Type();
  }

  if (dynamic_cast<TypeTestIs*>(expr)) {
    // Type test always returns bool
    return TypeReference("bool");
  }

  if (auto* assign = dynamic_cast<Assign*>(expr)) {
    // Assignment returns the type of the target
    return InferExpressionType(&assign->MutableTarget());
  }

  if (dynamic_cast<NamespaceRef*>(expr) != nullptr) {
    // NamespaceRef is used for system commands, return Object for now
    return TypeReference("Object");
  }

  // If we can't determine the type, return empty
  // This should not happen for literals as they are checked first
  return {};
}

bool TypeChecker::TypesCompatible(const TypeReference& expected, const TypeReference& actual) {
  if (IsSameType(expected, actual)) {
    return true;
  }

  // null (Object?) is compatible with any nullable type
  std::string actual_name;
  if (!actual.QualifiedName().empty()) {
    actual_name = std::string(actual.SimpleName());
  }
  if (actual_name == "Object" && actual.IsNullable()) {
    // null can be assigned to any nullable type
    if (expected.IsNullable()) {
      return true;
    }
  }

  // Non-nullable type can be implicitly converted to nullable
  if (expected.IsNullable() && !actual.IsNullable()) {
    TypeReference expected_non_nullable = expected.WithoutNullable();
    if (IsSameType(expected_non_nullable, actual) || IsImplicitlyConvertible(actual, expected_non_nullable)) {
      return true;
    }
    // Also check interface compatibility for non-nullable types
    if (!expected_non_nullable.QualifiedName().empty()) {
      std::string expected_non_nullable_name = std::string(expected_non_nullable.SimpleName());
      if (interfaces_.find(expected_non_nullable_name) != interfaces_.end()) {
        // Expected type is an interface (non-nullable)
        if (class_implements_.find(actual_name) != class_implements_.end()) {
          const auto& impls = class_implements_[actual_name];
          for (const auto& impl : impls) {
            if (impl == expected_non_nullable_name) {
              return true;
            }
          }
        }
      }
    }
  }

  // Check if a class implements an interface
  std::string expected_name;
  if (!expected.QualifiedName().empty()) {
    expected_name = std::string(expected.SimpleName());
  }
  if (!expected_name.empty() && !actual_name.empty() && !expected.IsNullable() && !actual.IsNullable()) {
    if (interfaces_.find(expected_name) != interfaces_.end()) {
      // Expected type is an interface
      // Check if it's the Object interface (all class types implicitly implement it)
      if (expected_name == "Object" && IsClassType(actual_name)) {
        return true;
      }
      // Check explicitly implemented interfaces (including IComparable, IHashable, IStringConvertible)
      if (class_implements_.find(actual_name) != class_implements_.end()) {
        const auto& impls = class_implements_[actual_name];
        for (const auto& impl : impls) {
          if (impl == expected_name) {
            return true;
          }
        }
      }
    }
  }

  return IsImplicitlyConvertible(actual, expected);
}

bool TypeChecker::IsSameType(const TypeReference& a, const TypeReference& b) {
  return a.StructurallyEquals(b);
}

bool TypeChecker::IsPrimitiveWrapper(const std::string& type_name) const {
  return type_name == "Int" || type_name == "Float" || type_name == "Byte" || type_name == "Char" ||
         type_name == "Bool";
}

bool TypeChecker::IsPrimitiveType(const std::string& type_name) const {
  return type_name == "int" || type_name == "float" || type_name == "byte" || type_name == "char" ||
         type_name == "bool";
}

bool TypeChecker::IsClassType(const std::string& type_name) const {
  // Primitives are not class types
  if (IsPrimitiveType(type_name)) {
    return false;
  }
  // All other types are class types (Int, Float, String, IntArray, user-defined classes, etc.)
  return true;
}

bool TypeChecker::IsImplicitlyConvertible(const TypeReference& from, const TypeReference& to) {
  std::string from_name;
  std::string to_name;

  if (!from.QualifiedName().empty()) {
    from_name = std::string(from.SimpleName());
  }
  if (!to.QualifiedName().empty()) {
    to_name = std::string(to.SimpleName());
  }

  if (from_name.empty() || to_name.empty()) {
    return false;
  }

  if (from_name == "int" && to_name == "Int") {
    return true;
  }
  if (from_name == "Int" && to_name == "int") {
    return true;
  }
  if (from_name == "float" && to_name == "Float") {
    return true;
  }
  if (from_name == "Float" && to_name == "float") {
    return true;
  }
  if (from_name == "byte" && to_name == "Byte") {
    return true;
  }
  if (from_name == "Byte" && to_name == "byte") {
    return true;
  }
  if (from_name == "char" && to_name == "Char") {
    return true;
  }
  if (from_name == "Char" && to_name == "char") {
    return true;
  }
  if (from_name == "bool" && to_name == "Bool") {
    return true;
  }
  if (from_name == "Bool" && to_name == "bool") {
    return true;
  }

  return false;
}

void TypeChecker::InitializeBuiltinMethods() {
  // String methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["String"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["String"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["String"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["String"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("ByteArray");
    builtin_methods_["String"]["ToUtf8Bytes"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["String"]["Substring"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("String")};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["String"]["Compare"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["String"]["IsLess"] = std::move(sig);
  }

  // Int methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["Int"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["Int"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Int"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Int")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Int"]["IsLess"] = std::move(sig);
  }

  // Float methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["Float"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["Float"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Float"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Float"]["IsLess"] = std::move(sig);
  }

  // Byte methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["Byte"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["Byte"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Byte"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Byte"]["IsLess"] = std::move(sig);
  }

  // Char methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["Char"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["Char"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Char"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Char"]["IsLess"] = std::move(sig);
  }

  // Bool methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["Bool"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["Bool"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Bool"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["Bool"]["IsLess"] = std::move(sig);
  }

  // Array methods - IntArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["IntArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["IntArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["IntArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["IntArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["IntArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["IntArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["IntArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["IntArray"]["SetAt"] = std::move(sig);
  }

  // Similar for other array types - FloatArray, ByteArray, BoolArray, CharArray, StringArray, ObjectArray
  // FloatArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["FloatArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["FloatArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["FloatArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["FloatArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["FloatArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["FloatArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("float");
    builtin_methods_["FloatArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("float")};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("float")};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("float")};
    sig.return_type = nullptr; // void
    builtin_methods_["FloatArray"]["SetAt"] = std::move(sig);
  }

  // ByteArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ByteArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ByteArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["ByteArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ByteArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["ByteArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["ByteArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("byte");
    builtin_methods_["ByteArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("byte")};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("byte")};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("byte")};
    sig.return_type = nullptr; // void
    builtin_methods_["ByteArray"]["SetAt"] = std::move(sig);
  }

  // BoolArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["BoolArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["BoolArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["BoolArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["BoolArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["BoolArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["BoolArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["BoolArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("bool")};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("bool")};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("bool")};
    sig.return_type = nullptr; // void
    builtin_methods_["BoolArray"]["SetAt"] = std::move(sig);
  }

  // CharArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["CharArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["CharArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["CharArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["CharArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["CharArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["CharArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("char");
    builtin_methods_["CharArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("char")};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("char")};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("char")};
    sig.return_type = nullptr; // void
    builtin_methods_["CharArray"]["SetAt"] = std::move(sig);
  }

  // StringArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["StringArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["StringArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["StringArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["StringArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["StringArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["StringArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["StringArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("String")};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("String")};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("String")};
    sig.return_type = nullptr; // void
    builtin_methods_["StringArray"]["SetAt"] = std::move(sig);
  }

  // ObjectArray
  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ObjectArray"]["Length"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ObjectArray"]["Capacity"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["ObjectArray"]["ToString"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["ObjectArray"]["GetHash"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["ObjectArray"]["Equals"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["ObjectArray"]["IsLess"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["Clear"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["ShrinkToFit"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["Reserve"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["RemoveAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("Object");
    builtin_methods_["ObjectArray"]["GetAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("Object")};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["Add"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("Object")};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["InsertAt"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int"), TypeReference("Object")};
    sig.return_type = nullptr; // void
    builtin_methods_["ObjectArray"]["SetAt"] = std::move(sig);
  }

  // File methods
  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("String"), TypeReference("String")};
    sig.return_type = nullptr; // void
    builtin_methods_["File"]["Open"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = nullptr; // void
    builtin_methods_["File"]["Close"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["File"]["IsOpen"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = std::make_unique<TypeReference>("ByteArray");
    builtin_methods_["File"]["Read"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("ByteArray")};
    sig.return_type = nullptr; // void
    builtin_methods_["File"]["Write"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("String");
    builtin_methods_["File"]["ReadLine"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("String")};
    sig.return_type = nullptr; // void
    builtin_methods_["File"]["WriteLine"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {TypeReference("int")};
    sig.return_type = nullptr; // void
    builtin_methods_["File"]["Seek"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("int");
    builtin_methods_["File"]["Tell"] = std::move(sig);
  }

  {
    BuiltinMethodSignature sig;
    sig.param_types = {};
    sig.return_type = std::make_unique<TypeReference>("bool");
    builtin_methods_["File"]["Eof"] = std::move(sig);
  }

  // Initialize Object interface with destructor
  InterfaceSignature object_interface;
  object_interface.interface_name = "Object";
  MethodSignature dtor_sig;
  dtor_sig.class_name = "Object";
  dtor_sig.method_name = "<dtor>";
  dtor_sig.param_types = {};
  dtor_sig.return_type = std::make_unique<TypeReference>("Void");
  object_interface.methods["<dtor>"] = std::move(dtor_sig);
  interfaces_["Object"] = std::move(object_interface);

  // Initialize IComparable interface (Equals and IsLess)
  InterfaceSignature comparable_interface;
  comparable_interface.interface_name = "IComparable";
  MethodSignature equals_sig;
  equals_sig.class_name = "IComparable";
  equals_sig.method_name = "Equals";
  equals_sig.param_types = {TypeReference("Object")};
  equals_sig.return_type = std::make_unique<TypeReference>("bool");
  comparable_interface.methods["Equals"] = std::move(equals_sig);
  MethodSignature isless_sig;
  isless_sig.class_name = "IComparable";
  isless_sig.method_name = "IsLess";
  isless_sig.param_types = {TypeReference("Object")};
  isless_sig.return_type = std::make_unique<TypeReference>("bool");
  comparable_interface.methods["IsLess"] = std::move(isless_sig);
  interfaces_["IComparable"] = std::move(comparable_interface);

  // Initialize IHashable interface (GetHash)
  InterfaceSignature hashable_interface;
  hashable_interface.interface_name = "IHashable";
  MethodSignature gethash_sig;
  gethash_sig.class_name = "IHashable";
  gethash_sig.method_name = "GetHash";
  gethash_sig.param_types = {};
  gethash_sig.return_type = std::make_unique<TypeReference>("int");
  hashable_interface.methods["GetHash"] = std::move(gethash_sig);
  interfaces_["IHashable"] = std::move(hashable_interface);

  // Initialize IStringConvertible interface (ToString)
  InterfaceSignature string_convertible_interface;
  string_convertible_interface.interface_name = "IStringConvertible";
  MethodSignature tostring_sig;
  tostring_sig.class_name = "IStringConvertible";
  tostring_sig.method_name = "ToString";
  tostring_sig.param_types = {};
  tostring_sig.return_type = std::make_unique<TypeReference>("String");
  string_convertible_interface.methods["ToString"] = std::move(tostring_sig);
  interfaces_["IStringConvertible"] = std::move(string_convertible_interface);

  // Register which builtin types implement which interfaces
  // IComparable: all builtin wrapper types and arrays
  class_implements_["Int"].emplace_back("IComparable");
  class_implements_["Float"].emplace_back("IComparable");
  class_implements_["String"].emplace_back("IComparable");
  class_implements_["Bool"].emplace_back("IComparable");
  class_implements_["Char"].emplace_back("IComparable");
  class_implements_["Byte"].emplace_back("IComparable");
  class_implements_["IntArray"].emplace_back("IComparable");
  class_implements_["FloatArray"].emplace_back("IComparable");
  class_implements_["StringArray"].emplace_back("IComparable");
  class_implements_["BoolArray"].emplace_back("IComparable");
  class_implements_["ByteArray"].emplace_back("IComparable");
  class_implements_["CharArray"].emplace_back("IComparable");
  class_implements_["ObjectArray"].emplace_back("IComparable");

  // IHashable: all builtin wrapper types and arrays
  class_implements_["Int"].emplace_back("IHashable");
  class_implements_["Float"].emplace_back("IHashable");
  class_implements_["String"].emplace_back("IHashable");
  class_implements_["Bool"].emplace_back("IHashable");
  class_implements_["Char"].emplace_back("IHashable");
  class_implements_["Byte"].emplace_back("IHashable");
  class_implements_["IntArray"].emplace_back("IHashable");
  class_implements_["FloatArray"].emplace_back("IHashable");
  class_implements_["StringArray"].emplace_back("IHashable");
  class_implements_["BoolArray"].emplace_back("IHashable");
  class_implements_["ByteArray"].emplace_back("IHashable");
  class_implements_["CharArray"].emplace_back("IHashable");
  class_implements_["ObjectArray"].emplace_back("IHashable");

  // IStringConvertible: only builtin wrapper types (NOT arrays)
  class_implements_["Int"].emplace_back("IStringConvertible");
  class_implements_["Float"].emplace_back("IStringConvertible");
  class_implements_["String"].emplace_back("IStringConvertible");
  class_implements_["Bool"].emplace_back("IStringConvertible");
  class_implements_["Char"].emplace_back("IStringConvertible");
  class_implements_["Byte"].emplace_back("IStringConvertible");
}

const TypeChecker::FunctionOverload* TypeChecker::ResolveFunctionOverload(
    const std::string& func_name, const std::vector<std::unique_ptr<Expr>>& args) {
  auto overloads_it = function_overloads_.find(func_name);
  if (overloads_it == function_overloads_.end()) {
    return nullptr;
  }

  const auto& overloads = overloads_it->second;
  if (overloads.empty()) {
    return nullptr;
  }

  // If only one overload, check types and return it if compatible
  if (overloads.size() == 1) {
    if (overloads[0].param_types.size() != args.size()) {
      return nullptr;
    }
    // Check if types are compatible
    for (size_t i = 0; i < args.size(); ++i) {
      TypeReference arg_type = InferExpressionType(args[i].get());
      if (arg_type.QualifiedName().empty() || !TypesCompatible(overloads[0].param_types[i], arg_type)) {
        return nullptr;
      }
    }
    return &overloads[0];
  }

  // Get argument types
  std::vector<TypeReference> arg_types;
  arg_types.reserve(args.size());
  for (const auto& arg : args) {
    arg_types.push_back(InferExpressionType(arg.get()));
  }

  // Find the best matching overload
  int best_match_score = -1;
  size_t best_match_index = 0;
  bool found_match = false;

  for (size_t i = 0; i < overloads.size(); ++i) {
    const auto& overload = overloads[i];

    // Check argument count
    if (overload.param_types.size() != arg_types.size()) {
      continue;
    }

    // Score this overload based on type compatibility
    int score = 0;
    bool is_compatible = true;

    for (size_t j = 0; j < arg_types.size(); ++j) {
      const TypeReference& expected_type = overload.param_types[j];
      const TypeReference& actual_type = arg_types[j];

      // Skip if actual type is empty (couldn't be inferred)
      if (actual_type.QualifiedName().empty()) {
        is_compatible = false;
        break;
      }

      if (IsSameType(expected_type, actual_type)) {
        score += 2; // Exact match
      } else if (TypesCompatible(expected_type, actual_type)) {
        score += 1; // Compatible (e.g., int -> Int, float -> Float)
      } else {
        is_compatible = false;
        break;
      }
    }

    if (is_compatible && score > best_match_score) {
      best_match_score = score;
      best_match_index = i;
      found_match = true;
    }
  }

  if (found_match) {
    return &overloads[best_match_index];
  }

  return nullptr;
}

const TypeChecker::BuiltinMethodSignature* TypeChecker::FindBuiltinMethod(const std::string& type_name,
                                                                          const std::string& method_name) {
  auto type_it = builtin_methods_.find(type_name);
  if (type_it == builtin_methods_.end()) {
    return nullptr;
  }

  auto method_it = type_it->second.find(method_name);
  if (method_it == type_it->second.end()) {
    return nullptr;
  }

  return &method_it->second;
}

std::string TypeChecker::GetFundamentalTypeName(const TypeReference& type) {
  if (type.QualifiedName().empty()) {
    return "";
  }

  std::string name = std::string(type.SimpleName());

  // Convert wrapper types to primitives
  if (name == "Int") {
    return "int";
  }
  if (name == "Float") {
    return "float";
  }
  if (name == "Byte") {
    return "byte";
  }
  if (name == "Char") {
    return "char";
  }
  if (name == "Bool") {
    return "bool";
  }

  // Return as-is for primitives and other types
  return name;
}

TypeReference TypeChecker::GetElementTypeForArray(const TypeReference& array_type) {
  if (array_type.QualifiedName().empty()) {
    return {};
  }

  std::string type_name = std::string(array_type.SimpleName());

  if (type_name == "IntArray") {
    return TypeReference("int");
  }
  if (type_name == "FloatArray") {
    return TypeReference("float");
  }
  if (type_name == "StringArray") {
    return TypeReference("String");
  }
  if (type_name == "BoolArray") {
    return TypeReference("bool");
  }
  if (type_name == "ByteArray") {
    return TypeReference("byte");
  }
  if (type_name == "CharArray") {
    return TypeReference("char");
  }
  if (type_name == "ObjectArray") {
    return TypeReference("Object");
  }

  // Not an array type
  return {};
}

} // namespace ovum::compiler::parser
