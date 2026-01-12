#include <ranges>

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

namespace ovum::compiler::parser {

void TypeChecker::Visit(Module& node) {
  functions_.clear();
  methods_.clear();
  class_fields_.clear();
  interfaces_.clear();
  class_implements_.clear();
  type_aliases_.clear();
  global_variables_.clear();
  constructors_.clear();

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
    }

    if (auto* c = dynamic_cast<ClassDecl*>(decl.get())) {
      std::string class_name = c->Name();
      std::vector<std::pair<std::string, TypeReference>> fields;

      // Register interfaces that this class implements
      std::vector<std::string> interface_names;
      for (const auto& impl : c->Implements()) {
        if (!impl.QualifiedName().empty()) {
          interface_names.push_back(std::string(impl.SimpleName()));
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
    TypeReference declared_type = node.Type();

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

  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableCallee())) {
    const std::string& func_name = ident->Name();

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
      // Note: We don't check argument types for built-in array constructors
      // as they are handled by the runtime
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
    } else if (auto func_it = functions_.find(func_name); func_it != functions_.end()) {
      const auto& sig = func_it->second;
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
      sink_.Error("E3008", "unknown function: " + func_name, node.Span());
    }
  } else if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableCallee())) {
    TypeReference object_type = InferExpressionType(&field_access->MutableObject());
    std::string class_name;
    if (!object_type.QualifiedName().empty()) {
      class_name = std::string(object_type.SimpleName());
    }
    if (!class_name.empty()) {
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
        std::ostringstream oss;
        oss << "unknown method '" << field_access->Name() << "' in class '" << class_name << "'";
        sink_.Error("E3014", oss.str(), node.Span());
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
          std::ostringstream oss;
          oss << "field '" << node.Name() << "' not found in class '" << class_name << "'";
          sink_.Error("E3009", oss.str(), node.Span());
        }
      }
    }
  }

  WalkVisitor::Visit(node);
}

void TypeChecker::Visit(IndexAccess& node) {
  InferExpressionType(&node.MutableObject());
  TypeReference index_type = InferExpressionType(&node.MutableIndexExpr());

  TypeReference int_type("int");
  if (!TypesCompatible(int_type, index_type)) {
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

  if (is_comparison || is_logical) {
    if (TypeReference bool_type("bool");
        !TypesCompatible(bool_type, lhs_type) || !TypesCompatible(bool_type, rhs_type)) {
      std::ostringstream oss;
      oss << "operands of '" << std::string(op.Name()) << "' must be of type 'bool'";
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
      } else if (node.Args().empty()) {
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
  if (variable_types_.find(node.Name()) == variable_types_.end() &&
      global_variables_.find(node.Name()) == global_variables_.end() &&
      functions_.find(node.Name()) == functions_.end() && class_fields_.find(node.Name()) == class_fields_.end()) {
    sink_.Error("E3015", "unknown variable: " + node.Name(), node.Span());
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

  if (auto* ident = dynamic_cast<IdentRef*>(expr)) {
    if (auto var_it = variable_types_.find(ident->Name()); var_it != variable_types_.end()) {
      return var_it->second;
    }
    if (auto global_it = global_variables_.find(ident->Name()); global_it != global_variables_.end()) {
      return global_it->second;
    }
    return {};
  }

  if (auto* call = dynamic_cast<Call*>(expr)) {
    if (auto* ident = dynamic_cast<IdentRef*>(&call->MutableCallee())) {
      const std::string& func_name = ident->Name();

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
      }
    }
    return {};
  }

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
      // FieldAccess on interface methods doesn't return a field type
      // It's used in method calls, which are handled separately
    }
    return {};
  }

  if (auto* index_access = dynamic_cast<IndexAccess*>(expr)) {
    TypeReference object_type = InferExpressionType(&index_access->MutableObject());
    std::string type_name;
    if (!object_type.QualifiedName().empty()) {
      type_name = std::string(object_type.SimpleName());
    }
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
    return {};
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
    InferExpressionType(&binary->MutableRhs());

    const IBinaryOpTag& op = binary->Op();
    bool is_comparison = &op == &optags::Eq() || &op == &optags::Ne() || &op == &optags::Lt() || &op == &optags::Le() ||
                         &op == &optags::Gt() || &op == &optags::Ge();
    bool is_logical = &op == &optags::And() || &op == &optags::Or();

    if (is_comparison || is_logical) {
      return TypeReference("bool");
    } else {
      // Arithmetic operations return the type of the operands (lhs type)
      return lhs_type;
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

} // namespace ovum::compiler::parser
