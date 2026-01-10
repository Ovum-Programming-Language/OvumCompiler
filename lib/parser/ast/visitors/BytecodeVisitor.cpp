#include "BytecodeVisitor.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/base/Stmt.hpp"
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
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
#include "lib/parser/ast/nodes/stmts/VarDeclStmt.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

namespace {
// TODO: Implement proper field size calculation based on actual type sizes
// Currently uses a heuristic that assumes all fields are 8 bytes
// Should calculate based on type: int/float/byte = 8, bool = 1, object = 8 (pointer), etc.
size_t FieldSizeForType(const TypeReference& t) {
  (void) t;
  return 8;
}

// helper: normalize string for printing (no quotes inside)
std::string EscapeStringForEmit(const std::string& s) {
  // minimal escaping only for double quotes
  std::string out;
  out.reserve(s.size());
  for (char c : s) {
    if (c == '"') {
      out.push_back('\\');
      out.push_back('"');
    } else {
      out.push_back(c);
    }
  }
  return out;
}
} // namespace

BytecodeVisitor::BytecodeVisitor(std::ostream& output) : output_(output), next_function_id_(0) {
  pending_init_static_ = {};
}

void BytecodeVisitor::EmitIndent() {
  for (int i = 0; i < indent_level_; ++i) {
    output_ << kIndent;
  }
}

void BytecodeVisitor::EmitCommand(const std::string& command) {
  EmitIndent();
  output_ << command << "\n";
}

void BytecodeVisitor::EmitCommandWithInt(const std::string& command, int64_t value) {
  EmitIndent();
  output_ << command << " " << value << "\n";
}

void BytecodeVisitor::EmitCommandWithFloat(const std::string& command, double value) {
  EmitIndent();
  output_ << command << " " << value << "\n";
}

void BytecodeVisitor::EmitCommandWithBool(const std::string& command, bool value) {
  EmitIndent();
  output_ << command << " " << (value ? "true" : "false") << "\n";
}

void BytecodeVisitor::EmitCommandWithString(const std::string& command, const std::string& value) {
  EmitIndent();
  output_ << command << " \"" << EscapeStringForEmit(value) << "\"\n";
}

void BytecodeVisitor::EmitCommandWithStringWithoutBraces(const std::string& command, const std::string& value) {
  EmitIndent();
  output_ << command << " " << EscapeStringForEmit(value) << "\n";
}

void BytecodeVisitor::EmitDup() {
  EmitCommand("Dup");
}

void BytecodeVisitor::EmitSwap() {
  EmitCommand("Swap");
}

void BytecodeVisitor::EmitRotate(int64_t n) {
  EmitCommandWithInt("Rotate", n);
}

// Helper method for PushByte - emits PushByte command for Byte literals
void BytecodeVisitor::EmitPushByte(uint8_t value) {
  EmitCommandWithInt("PushByte", static_cast<int64_t>(value));
}

// Helper methods for low-level vtable operations (for unsafe blocks)
void BytecodeVisitor::EmitGetVTable(const std::string& class_name) {
  EmitCommandWithStringWithoutBraces("GetVTable", class_name);
}

void BytecodeVisitor::EmitSetVTable(const std::string& class_name) {
  EmitCommandWithStringWithoutBraces("SetVTable", class_name);
}

void BytecodeVisitor::EmitBlockStart() {
  EmitIndent();
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::EmitBlockStartWithoutSpaces() {
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::EmitBlockEnd() {
  indent_level_--;
  EmitIndent();
  output_ << "}\n";
}

void BytecodeVisitor::EmitBlockEndWithoutEscape() {
  indent_level_--;
  EmitIndent();
  output_ << "}";
}

void BytecodeVisitor::EmitIfStart() {
  EmitIndent();
  output_ << "if ";
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::EmitThenStart() {
  output_ << " then ";
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::EmitElseStart() {
  output_ << " else ";
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::EmitElseIfStart() {
  EmitIndent();
  output_ << "else if ";
  output_ << "{\n";
  indent_level_++;
}

void BytecodeVisitor::Visit(Module& node) {
  // First pass: register function/method names and collect static initializers
  // We need function registry so that later Call nodes can emit mangled names.
  // TODO: Consider using a symbol table instead of multiple maps for better organization
  function_name_map_.clear();
  function_return_types_.clear();
  method_name_map_.clear();
  method_vtable_map_.clear();
  method_return_types_.clear();
  class_fields_.clear();
  pending_init_static_.clear();

  // scan declarations to register names and collect static inits
  for (auto& decl : node.MutableDecls()) {
    // function declarations
    if (auto* f = dynamic_cast<FunctionDecl*>(decl.get())) {
      std::string mangled = GenerateFunctionId(f->Name(), f->Params());
      function_name_map_[f->Name()] = mangled;
      // Store return type
      if (f->ReturnType() != nullptr) {
        function_return_types_[f->Name()] = TypeToMangledName(*f->ReturnType());
      } else {
        function_return_types_[f->Name()] = "void";
      }
    }
    // class declarations: register methods and collect fields
    if (auto* c = dynamic_cast<ClassDecl*>(decl.get())) {
      std::string class_name = c->Name();
      // collect fields and method mangles
      std::vector<std::pair<std::string, TypeReference>> fields;
      for (auto& m : c->MutableMembers()) {
        if (auto* fd = dynamic_cast<FieldDecl*>(m.get())) {
          fields.emplace_back(fd->Name(), fd->Type()); // assumes FieldDecl has GetType()
        }
        if (auto* sd = dynamic_cast<StaticFieldDecl*>(m.get())) {
          // static field initializer -> add to pending_init_static_
          if (sd->MutableInit() != nullptr) {
            pending_init_static_.push_back(sd->MutableInit());
            pending_init_static_names_.push_back(sd->Name());
          }
        }
        if (auto* md = dynamic_cast<MethodDecl*>(m.get())) {
          // Check if this is a constructor (method with same name as class)
          if (md->Name() == class_name) {
            // Register as constructor
            std::string ctor = GenerateConstructorId(class_name, md->Params());
            method_name_map_[class_name + "::<ctor>"] = ctor;
          } else {
            // Normal method
            bool is_mutable = false; // TODO: md->IsMutable() when available
            std::string mid = GenerateMethodId(class_name, md->Name(), md->Params(), false, false, is_mutable);
            std::string vtable_name = GenerateMethodVTableName(md->Name(), md->Params(), is_mutable);
            method_name_map_[class_name + "::" + md->Name()] = mid;
            method_vtable_map_[class_name + "::" + md->Name()] = vtable_name;
            // Store return type
            if (md->ReturnType() != nullptr) {
              method_return_types_[class_name + "::" + md->Name()] = TypeToMangledName(*md->ReturnType());
            } else {
              method_return_types_[class_name + "::" + md->Name()] = "void";
            }
          }
        }
        if (auto* cd = dynamic_cast<CallDecl*>(m.get())) {
          std::string ctor = GenerateConstructorId(class_name, cd->Params());
          method_name_map_[class_name + "::<ctor>"] = ctor;
        }
      }
      class_fields_[class_name] = fields;
    }
    // global/static var declarations to init-static
    if (auto* gv = dynamic_cast<GlobalVarDecl*>(decl.get())) {
      if (gv->MutableInit() != nullptr) {
        pending_init_static_.push_back(gv->MutableInit());
        pending_init_static_names_.push_back(gv->Name());
      }
    }
  }

  // Emit a single init-static block even if empty (match examples)
  EmitIndent();
  output_ << "init-static {\n";
  indent_level_++;
  if (!pending_init_static_.empty()) {
    for (size_t i = 0; i < pending_init_static_.size(); ++i) {
      // Evaluate initializer expression and SetStatic with assigned index
      pending_init_static_[i]->Accept(*this);
      EmitCommandWithInt("SetStatic", static_cast<int64_t>(GetStaticIndex(pending_init_static_names_[i])));
    }
  }
  indent_level_--;
  EmitIndent();
  output_ << "}\n\n";

  // Second pass: actually emit declarations (functions, classes, etc.)
  for (auto& decl : node.MutableDecls()) {
    decl->Accept(*this);
  }
}

void BytecodeVisitor::Visit(FunctionDecl& node) {
  ResetLocalVariables();

  // Track current function name for return statement wrapping
  std::string prev_function = current_function_name_;
  current_function_name_ = node.Name();

  // Register parameters as local variables (they start at index 0, 1, 2, ...)
  // Parameters are pushed right-to-left, so LoadLocal 0 = first (leftmost) parameter
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());
    // Store parameter type for operand type determination
    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  // Register function in function map (if not already registered in first pass)
  std::string mangled = GenerateFunctionId(node.Name(), node.Params());

  // Store return type for return statement wrapping
  if (node.ReturnType() != nullptr) {
    function_return_types_[node.Name()] = TypeToMangledName(*node.ReturnType());
  } else {
    function_return_types_[node.Name()] = "void";
  }

  // Emit function declaration
  // Format: pure(<param_types>)? function:<param_count> <mangled_name> { ... }
  // According to documentation: pure(int, int) function:2 _Global_IsEven_int_int
  // The parentheses contain parameter types, not return type
  if (node.IsPure()) {
    output_ << "pure";
    // Generate parameter types in parentheses
    if (!node.Params().empty()) {
      output_ << "(";
      for (size_t i = 0; i < node.Params().size(); ++i) {
        if (i > 0) {
          output_ << ", ";
        }
        std::string param_type_name = TypeToMangledName(node.Params()[i].GetType());
        output_ << param_type_name;
      }
      output_ << ")";
    } else {
      // No parameters - empty parentheses
      output_ << "()";
    }
    output_ << " ";
  }
  output_ << "function:" << node.Params().size() << " " << mangled << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";

  // Restore previous function name
  current_function_name_ = prev_function;
}

void BytecodeVisitor::Visit(MethodDecl& node) {
  // Check if this is actually a constructor (method with same name as class)
  // If so, handle it as a constructor
  if (node.Name() == current_class_name_) {
    ResetLocalVariables();
    GetLocalIndex("this");
    for (auto& param : node.MutableParams()) {
      GetLocalIndex(param.GetName());
      // Store parameter type for operand type determination
      std::string type_name = TypeToMangledName(param.GetType());
      variable_types_[param.GetName()] = type_name;
    }

    std::string ctor_id = GenerateConstructorId(current_class_name_, node.Params());
    method_name_map_[current_class_name_ + "::<ctor>"] = ctor_id;

    // Constructor has this + params.size() parameters
    output_ << "function:" << (node.Params().size() + 1) << " " << ctor_id << " ";
    EmitBlockStart();
    if (node.MutableBody() != nullptr) {
      // Check if body already ends with a return statement
      bool has_return = false;
      auto& stmts = node.MutableBody()->GetStatements();
      if (!stmts.empty()) {
        if (dynamic_cast<ReturnStmt*>(stmts.back().get()) != nullptr) {
          has_return = true;
        }
      }
      
      node.MutableBody()->Accept(*this);
      
      // Only add return this if body doesn't already return
      if (!has_return) {
        EmitCommandWithInt("LoadLocal", 0);
        EmitCommand("Return");
      }
    } else {
      EmitCommandWithInt("LoadLocal", 0);
      EmitCommand("Return");
    }
    EmitBlockEnd();
    output_ << "\n";
    return;
  }

  // Normal method processing
  ResetLocalVariables();
  // TODO: Handle 'this' parameter for instance methods (should be at index 0)
  // Currently methods don't have 'this' in parameter list, but should access it as LoadLocal 0
  // This might need adjustment based on how methods are called
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());
    // Store parameter type for operand type determination
    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  // Determine if method is mutable
  // TODO: Add IsMutable() method to MethodDecl when mutable methods are supported
  // For now, assume all methods are const (<C>)
  bool is_mutable = false; // TODO: node.IsMutable() when available

  std::string method_id = GenerateMethodId(current_class_name_, node.Name(), node.Params(), false, false, is_mutable);
  // register mapping class::name -> mangled
  method_name_map_[current_class_name_ + "::" + node.Name()] = method_id;

  // Note: Parameter count doesn't include 'this' for methods
  output_ << "function:" << node.Params().size() << " " << method_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(CallDecl& node) {
  ResetLocalVariables();
  // Constructor has 'this' as first parameter (LoadLocal 0)
  // Parameters start from LoadLocal 1, 2, etc.
  // So we register 'this' at index 0, then parameters at 1, 2, etc.
  GetLocalIndex("this"); // Register 'this' at index 0
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());
    // Store parameter type for operand type determination
    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  std::string call_id = GenerateConstructorId(current_class_name_, node.Params());
  // store constructor mapping as Class::<ctor>
  method_name_map_[current_class_name_ + "::<ctor>"] = call_id;

  // Constructor has this + params.size() parameters
  output_ << "function:" << (node.Params().size() + 1) << " " << call_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
    // Constructors should return this if body doesn't already return
    // Check if last statement is return - for now, always add return this
    EmitCommandWithInt("LoadLocal", 0);
    EmitCommand("Return");
  } else {
    // If no body, return this
    EmitCommandWithInt("LoadLocal", 0);
    EmitCommand("Return");
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(InterfaceDecl& node) {
  for (auto& m : node.MutableMembers()) {
    m->Accept(*this);
  }
}

void BytecodeVisitor::Visit(InterfaceMethod& node) {
  bool is_mutable = false; // TODO: node.IsMutable() when available
  std::string id = GenerateMethodId(
      /* class_name */ "",
      node.Name(),
      node.MutableParams(),
      /* is_constructor */ false,
      /* is_destructor */ false,
      is_mutable);

  method_name_map_[node.Name()] = id;
}

void BytecodeVisitor::Visit(TypeAliasDecl&) {
}

void BytecodeVisitor::Visit(GlobalVarDecl& node) {
  // GlobalVarDecl initialization is already handled in init-static block
  // during the first pass in Visit(Module&). This method is called during
  // the second pass and should not generate any code - just ensure the
  // variable is registered in static index map.
  (void) GetStaticIndex(node.Name());
}

void BytecodeVisitor::Visit(FieldDecl&) {
}

void BytecodeVisitor::Visit(StaticFieldDecl& node) {
  if (node.MutableInit()) {
    pending_init_static_.push_back(node.MutableInit());
    pending_init_static_names_.push_back(node.Name());
  }
}

void BytecodeVisitor::Visit(DestructorDecl& node) {
  ResetLocalVariables();
  // Destructor has 'this' as first parameter (LoadLocal 0)
  GetLocalIndex("this"); // Register 'this' at index 0

  std::string destructor_id = GenerateDestructorId(current_class_name_);
  method_name_map_[current_class_name_ + "::<dtor>"] = destructor_id;

  // Destructor has 1 parameter: this
  output_ << "function:1 " << destructor_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(ClassDecl& node) {
  // collect fields and interfaces to build vtable
  std::string prev_class = current_class_name_;
  current_class_name_ = node.Name();

  // Collect fields
  std::vector<std::pair<std::string, TypeReference>> fields;
  std::vector<std::pair<std::string, std::string>> method_map; // methodName -> mangled

  for (auto& member : node.MutableMembers()) {
    if (auto* fd = dynamic_cast<FieldDecl*>(member.get())) {
      fields.emplace_back(fd->Name(), fd->Type());
    }
    if (auto* ifm = dynamic_cast<InterfaceMethod*>(member.get())) {
      // interface methods in class? often not present in members; skip
      (void) ifm;
    }
    if (auto* md = dynamic_cast<MethodDecl*>(member.get())) {
      // Skip methods with the same name as the class - these are constructors
      // and should be handled as CallDecl, not MethodDecl
      if (md->Name() == node.Name()) {
        // This is actually a constructor, treat it as such
        std::string ctor_id = GenerateConstructorId(node.Name(), md->Params());
        method_name_map_[node.Name() + "::<ctor>"] = ctor_id;
        // Don't add to method_map - constructors are not in vtable
      } else {
        bool is_mutable = false; // TODO: md->IsMutable() when available
        std::string mid = GenerateMethodId(node.Name(), md->Name(), md->Params(), false, false, is_mutable);
        std::string vtable_name = GenerateMethodVTableName(md->Name(), md->Params(), is_mutable);
        // Store method name and params for vtable generation
        method_map.emplace_back(md->Name(), mid);
        method_name_map_[node.Name() + "::" + md->Name()] = mid;
        method_vtable_map_[node.Name() + "::" + md->Name()] = vtable_name;
      }
    }
    if (auto* cd = dynamic_cast<CallDecl*>(member.get())) {
      std::string ce = GenerateConstructorId(node.Name(), cd->Params());
      // Constructors are not in vtable methods, only store for CallConstructor
      method_name_map_[node.Name() + "::<ctor>"] = ce;
    }
    if (auto* sfd = dynamic_cast<StaticFieldDecl*>(member.get())) {
      // static field handled in module init-phase earlier
      if (sfd->MutableInit() != nullptr) {
        pending_init_static_.push_back(sfd->MutableInit());
        pending_init_static_names_.push_back(sfd->Name());
      }
    }
  }

  // compute size: 4 (vtable idx) + 4 (badge) + sum(field sizes)
  size_t header = 4 + 4;
  size_t fields_size = 0;
  for (auto& f : fields) {
    fields_size += FieldSizeForType(f.second);
  }
  size_t total_size = header + fields_size;

  // Emit vtable block
  EmitIndent();
  output_ << "vtable " << node.Name() << " {\n";
  indent_level_++;
  EmitIndent();
  output_ << "size: " << total_size << "\n";

  if (!node.MutableImplements().empty()) {
    EmitIndent();
    output_ << "interfaces {\n";
    indent_level_++;
    for (const auto& iface : node.MutableImplements()) {
      EmitIndent();
      output_ << iface.SimpleName() << "\n";
    }
    indent_level_--;
    EmitIndent();
    output_ << "}\n";
  } else {
    // empty interfaces block omitted in your examples, but keep consistent
  }

  // methods (constructors are not in vtable, only regular methods)
  if (!method_map.empty()) {
    EmitIndent();
    output_ << "methods {\n";
    indent_level_++;
    for (auto& m : method_map) {
      EmitIndent();
      // Format: _methodName_<C>_args: _ClassName_methodName_<C>_args
      // Find the MethodDecl to get params for vtable name
      std::string vtable_name;
      for (auto& member : node.MutableMembers()) {
        if (auto* md = dynamic_cast<MethodDecl*>(member.get())) {
          if (md->Name() == m.first) {
            bool is_mutable = false; // TODO: md->IsMutable() when available
            vtable_name = GenerateMethodVTableName(m.first, md->Params(), is_mutable);
            break;
          }
        }
      }
      output_ << vtable_name << ": " << m.second << "\n";
    }
    indent_level_--;
    EmitIndent();
    output_ << "}\n";
  }

  // vartable (fields)
  EmitIndent();
  output_ << "vartable {\n";
  indent_level_++;
  // compute offsets: after header
  size_t offset = header;
  for (auto& f : fields) {
    EmitIndent();
    // format type@offset — we show "Object" for user-defined if ambiguous
    std::string tname = TypeToMangledName(f.second);
    output_ << f.first << ": " << tname << "@" << offset << "\n";
    offset += FieldSizeForType(f.second);
  }
  indent_level_--;
  EmitIndent();
  output_ << "}\n";

  indent_level_--;
  EmitIndent();
  output_ << "}\n\n";

  // Emit members (methods/constructors/destructors) bodies
  for (auto& member : node.MutableMembers()) {
    member->Accept(*this);
  }

  current_class_name_ = prev_class;
}

void BytecodeVisitor::Visit(Block& node) {
  for (auto& stmt : node.GetStatements()) {
    stmt->Accept(*this);
  }
}

void BytecodeVisitor::Visit(VarDeclStmt& node) {
  if (node.MutableInit() != nullptr) {
    node.MutableInit()->Accept(*this);
    EmitCommandWithInt("SetLocal", static_cast<int64_t>(GetLocalIndex(node.Name())));
  }
  // Store variable type for later use
  std::string type_name = TypeToMangledName(node.Type());
  variable_types_[node.Name()] = type_name;
}

void BytecodeVisitor::Visit(ExprStmt& node) {
  if (node.MutableExpression() != nullptr) {
    node.MutableExpression()->Accept(*this);

    // Only pop if expression returns a value (not void)
    // Check if expression is a Call that returns void
    bool should_pop = true;

    if (auto* call = dynamic_cast<Call*>(node.MutableExpression())) {
      // Try to determine return type from function name
      if (auto* ident = dynamic_cast<IdentRef*>(&call->MutableCallee())) {
        std::string func_name = ident->Name();
        auto return_type_it = function_return_types_.find(func_name);
        if (return_type_it != function_return_types_.end()) {
          // Check if function returns void
          if (return_type_it->second == "void" || return_type_it->second == "Void") {
            should_pop = false;
          }
        }
      } else if (auto* field_access = dynamic_cast<FieldAccess*>(&call->MutableCallee())) {
        // Method call - check if method returns void
        std::string method_name = field_access->Name();
        // Try to determine object type to find method
        std::string object_type;
        if (auto* obj_ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
          auto type_it = variable_types_.find(obj_ident->Name());
          if (type_it != variable_types_.end()) {
            object_type = type_it->second;
          }
        }

        // Search for method return type
        std::string method_key;
        if (!object_type.empty()) {
          method_key = object_type + "::" + method_name;
        } else if (!current_class_name_.empty()) {
          method_key = current_class_name_ + "::" + method_name;
        }

        if (!method_key.empty()) {
          auto return_type_it = method_return_types_.find(method_key);
          if (return_type_it != method_return_types_.end()) {
            if (return_type_it->second == "void" || return_type_it->second == "Void") {
              should_pop = false;
            }
          }
        }
      }
    } else if (auto* assign = dynamic_cast<Assign*>(node.MutableExpression())) {
      // Assignment expressions return the assigned value
      should_pop = true;
    } else {
      // Other expressions (literals, binary, unary, etc.) return values
      should_pop = true;
    }

    if (should_pop) {
      EmitCommand("Pop");
    }
  }
}

void BytecodeVisitor::Visit(ReturnStmt& node) {
  if (node.MutableValue() != nullptr) {
    node.MutableValue()->Accept(*this);

    // If return type is a wrapper and expression result is primitive, wrap it
    if (!current_function_name_.empty()) {
      auto it = function_return_types_.find(current_function_name_);
      if (it != function_return_types_.end()) {
        std::string return_type = it->second;
        OperandType result_type = DetermineOperandType(node.MutableValue());
        if (IsPrimitiveWrapper(return_type) &&
            (result_type == OperandType::Int || result_type == OperandType::Float || result_type == OperandType::Byte ||
             result_type == OperandType::Bool || result_type == OperandType::Char)) {
          EmitWrapIfNeeded(return_type, result_type);
        }
      }
    }
  }
  EmitCommand("Return");
}

void BytecodeVisitor::Visit(BreakStmt& /*node*/) {
  EmitCommand("Break");
}

void BytecodeVisitor::Visit(ContinueStmt& /*node*/) {
  EmitCommand("Continue");
}

void BytecodeVisitor::Visit(IfStmt& node) {
  auto& branches = node.MutableBranches();

  for (size_t i = 0; i < branches.size(); ++i) {
    auto* cond = branches[i].MutableCondition();
    if (!cond) {
      continue;
    }

    if (i == 0) {
      EmitIfStart();
    } else {
      EmitElseIfStart();
    }

    cond->Accept(*this);
    EmitBlockEndWithoutEscape();

    EmitThenStart();
    branches[i].MutableThen()->Accept(*this);
    if (auto* else_block = node.MutableElseBlock()) {
      EmitBlockEndWithoutEscape();
    } else {
      EmitBlockEnd();
    }
  }

  if (auto* else_block = node.MutableElseBlock()) {
    EmitElseStart();
    else_block->Accept(*this);
    EmitBlockEnd();
  }
}

void BytecodeVisitor::Visit(WhileStmt& node) {
  EmitIndent();
  output_ << "while ";
  EmitBlockStartWithoutSpaces();
  if (node.MutableCondition() != nullptr) {
    node.MutableCondition()->Accept(*this);
  }
  EmitBlockEndWithoutEscape();
  output_ << " then ";
  EmitBlockStartWithoutSpaces();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
}

void BytecodeVisitor::Visit(ForStmt& node) {
  // Lower for loop: for (item in collection) { body }
  // Translates to: collection.iterator(), then while loop with MoveNext() and Current

  // Get iterator from collection
  if (node.MutableIteratorExpr() != nullptr) {
    node.MutableIteratorExpr()->Accept(*this);
    // Call GetIterator method on collection (assumes collection has GetIterator())
    EmitCommandWithStringWithoutBraces("Call", "_Object_GetIterator_<C>");
  }

  // Store iterator in a local variable
  size_t iter_index = GetLocalIndex(node.IteratorName() + "_iter");
  EmitCommandWithInt("SetLocal", static_cast<int64_t>(iter_index));

  // Create while loop: while (iterator.MoveNext()) { var item = iterator.Current; body }
  EmitIndent();
  output_ << "while ";
  EmitBlockStartWithoutSpaces();

  // Check MoveNext: iterator.MoveNext()
  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(iter_index));
  EmitCommandWithStringWithoutBraces("Call", "_Iterator_MoveNext_<C>");

  EmitBlockEndWithoutEscape();
  output_ << " then ";
  EmitBlockStartWithoutSpaces();

  // Get current item: iterator.Current
  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(iter_index));
  EmitCommandWithStringWithoutBraces("Call", "_Iterator_Current_<C>");

  // Store in iterator variable
  size_t item_index = GetLocalIndex(node.IteratorName());
  EmitCommandWithInt("SetLocal", static_cast<int64_t>(item_index));

  // Emit loop body
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }

  EmitBlockEnd();
}

void BytecodeVisitor::Visit(UnsafeBlock& node) {
  // Unsafe blocks allow low-level operations that bypass safety guarantees
  // Inside unsafe blocks, operations like GetVTable/SetVTable can be used
  // for low-level object manipulation
  // Note: GetVTable/SetVTable can be called via sys::GetVTable/sys::SetVTable
  // or through direct bytecode generation if needed
  EmitCommand("UnsafeBlockStart");
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitCommand("UnsafeBlockEnd");
}

void BytecodeVisitor::Visit(Binary& node) {
  // Binary operations: push operands right-to-left, then emit operation command
  // Special handling for null comparisons (== null, != null) using IsNull command

  const auto& op = node.Op();

  // Check for null comparison before pushing operands
  // Null comparisons are optimized to use IsNull command directly
  bool is_null_comparison = (dynamic_cast<NullLit*>(&node.MutableLhs()) != nullptr) ||
                            (dynamic_cast<NullLit*>(&node.MutableRhs()) != nullptr);

  if (is_null_comparison && (&op == &OpTags::Eq() || &op == &OpTags::Ne())) {
    // For null comparison, push only the non-null operand, then check IsNull
    if (dynamic_cast<NullLit*>(&node.MutableLhs()) != nullptr) {
      // lhs is null, rhs is the expression to check
      node.MutableRhs().Accept(*this);
    } else {
      // rhs is null, lhs is the expression to check
      node.MutableLhs().Accept(*this);
    }
    // Stack now: [value] - check if it's null
    EmitCommand("IsNull");
    if (&op == &OpTags::Ne()) {
      // For != null, negate the result
      EmitCommand("BoolNot");
    }
    return;
  }

  // Push operands right-to-left (right operand first, left operand last)
  // According to bytecode spec: operands are pushed right-to-left
  // This ensures correct stack order for binary operations
  // Check if operands are primitive wrappers and unwrap them if needed
  std::string lhs_type_name = GetTypeNameForExpr(&node.MutableLhs());
  std::string rhs_type_name = GetTypeNameForExpr(&node.MutableRhs());

  // Determine operation type based on operand types (before unwrapping)
  // After unwrapping, primitives will be on stack, so we need to know the primitive type
  // GetPrimitiveTypeForWrapper converts wrapper types to primitives for operation selection
  std::string lhs_primitive =
      IsPrimitiveWrapper(lhs_type_name) ? GetPrimitiveTypeForWrapper(lhs_type_name) : lhs_type_name;
  std::string rhs_primitive =
      IsPrimitiveWrapper(rhs_type_name) ? GetPrimitiveTypeForWrapper(rhs_type_name) : rhs_type_name;

  OperandType lhs_type = DetermineOperandType(&node.MutableLhs());
  OperandType rhs_type = DetermineOperandType(&node.MutableRhs());

  // After unwrapping, both operands become primitives, so we need to determine
  // the primitive type for operation selection
  if (IsPrimitiveWrapper(lhs_type_name)) {
    // Convert wrapper to primitive type for operation
    if (lhs_type_name == "Int")
      lhs_type = OperandType::Int;
    else if (lhs_type_name == "Float")
      lhs_type = OperandType::Float;
    else if (lhs_type_name == "Byte")
      lhs_type = OperandType::Byte;
    else if (lhs_type_name == "Char")
      lhs_type = OperandType::Char;
    else if (lhs_type_name == "Bool")
      lhs_type = OperandType::Bool;
  }
  if (IsPrimitiveWrapper(rhs_type_name)) {
    // Convert wrapper to primitive type for operation
    if (rhs_type_name == "Int")
      rhs_type = OperandType::Int;
    else if (rhs_type_name == "Float")
      rhs_type = OperandType::Float;
    else if (rhs_type_name == "Byte")
      rhs_type = OperandType::Byte;
    else if (rhs_type_name == "Char")
      rhs_type = OperandType::Char;
    else if (rhs_type_name == "Bool")
      rhs_type = OperandType::Bool;
  }

  // Determine the dominant type (Float > Int > Byte > Bool)
  // For comparison and bitwise operations, if one operand is Byte, prefer Byte operations
  bool is_comparison = (&op == &OpTags::Lt() || &op == &OpTags::Le() || &op == &OpTags::Gt() || &op == &OpTags::Ge() ||
                        &op == &OpTags::Eq() || &op == &OpTags::Ne());
  bool is_bitwise = (&op == &OpTags::BitwiseAnd() || &op == &OpTags::BitwiseOr() || &op == &OpTags::Xor() ||
                     &op == &OpTags::LeftShift() || &op == &OpTags::RightShift());

  OperandType dominant_type = lhs_type;
  if (rhs_type == OperandType::Float || lhs_type == OperandType::Float) {
    dominant_type = OperandType::Float;
  } else if ((is_comparison || is_bitwise) && (rhs_type == OperandType::Byte || lhs_type == OperandType::Byte)) {
    // For comparisons and bitwise operations, if one operand is Byte, use Byte operations
    dominant_type = OperandType::Byte;
  } else if (rhs_type == OperandType::Int || lhs_type == OperandType::Int) {
    dominant_type = OperandType::Int;
  } else if (rhs_type == OperandType::Byte || lhs_type == OperandType::Byte) {
    dominant_type = OperandType::Byte;
  } else if (rhs_type == OperandType::String || lhs_type == OperandType::String) {
    dominant_type = OperandType::String;
  } else if (rhs_type == OperandType::Bool || lhs_type == OperandType::Bool) {
    dominant_type = OperandType::Bool;
  }

  // For StringConcat, push operands left-to-right (left first, right last)
  // because StringConcat does left + right and expects [left, right] on stack
  // For other operations, push right-to-left (right first, left last)
  if (&op == &OpTags::Add() && dominant_type == OperandType::String) {
    // String concatenation: push left first, then right
    node.MutableLhs().Accept(*this);
    if (IsPrimitiveWrapper(lhs_type_name)) {
      EmitCommand("Unwrap");
    }

    node.MutableRhs().Accept(*this);
    if (IsPrimitiveWrapper(rhs_type_name)) {
      EmitCommand("Unwrap");
    }

    EmitCommand("StringConcat");
    return;
  } else {
    // For non-string operations, push operands right-to-left (right first, left last)
    node.MutableRhs().Accept(*this);
    if (IsPrimitiveWrapper(rhs_type_name)) {
      EmitCommand("Unwrap");
    }

    node.MutableLhs().Accept(*this);
    if (IsPrimitiveWrapper(lhs_type_name)) {
      EmitCommand("Unwrap");
    }

    if (&op == &OpTags::Add()) {
      // Non-string addition (already handled StringConcat above)
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatAdd");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteAdd");
      } else {
        EmitCommand("IntAdd");
      }
    } else if (&op == &OpTags::Sub()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatSubtract");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteSubtract");
      } else {
        EmitCommand("IntSubtract");
      }
    } else if (&op == &OpTags::Mul()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatMultiply");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteMultiply");
      } else {
        EmitCommand("IntMultiply");
      }
    } else if (&op == &OpTags::Div()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatDivide");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteDivide");
      } else {
        EmitCommand("IntDivide");
      }
    } else if (&op == &OpTags::Mod()) {
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteModulo");
      } else {
        EmitCommand("IntModulo");
      }
    } else if (&op == &OpTags::Lt()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatLessThan");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteLessThan");
      } else {
        EmitCommand("IntLessThan");
      }
    } else if (&op == &OpTags::Le()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatLessEqual");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteLessEqual");
      } else {
        EmitCommand("IntLessEqual");
      }
    } else if (&op == &OpTags::Gt()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatGreaterThan");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteGreaterThan");
      } else {
        EmitCommand("IntGreaterThan");
      }
    } else if (&op == &OpTags::Ge()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatGreaterEqual");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteGreaterEqual");
      } else {
        EmitCommand("IntGreaterEqual");
      }
    } else if (&op == &OpTags::Eq()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatEqual");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteEqual");
      } else {
        EmitCommand("IntEqual");
      }
    } else if (&op == &OpTags::Ne()) {
      if (dominant_type == OperandType::Float) {
        EmitCommand("FloatNotEqual");
      } else if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteNotEqual");
      } else {
        EmitCommand("IntNotEqual");
      }
    } else if (&op == &OpTags::And()) {
      // Logical AND (&&)
      EmitCommand("BoolAnd");
    } else if (&op == &OpTags::Or()) {
      // Logical OR (||)
      EmitCommand("BoolOr");
    } else if (&op == &OpTags::Xor()) {
      // Xor can be both logical (BoolXor) and bitwise (IntXor/ByteXor)
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteXor");
      } else if (dominant_type == OperandType::Int) {
        EmitCommand("IntXor");
      } else {
        EmitCommand("BoolXor");
      }
    } else if (&op == &OpTags::BitwiseAnd()) {
      // Bitwise AND (&)
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteAnd");
      } else {
        EmitCommand("IntAnd");
      }
    } else if (&op == &OpTags::BitwiseOr()) {
      // Bitwise OR (|)
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteOr");
      } else {
        EmitCommand("IntOr");
      }
    } else if (&op == &OpTags::LeftShift()) {
      // Left shift (<<)
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteLeftShift");
      } else {
        EmitCommand("IntLeftShift");
      }
    } else if (&op == &OpTags::RightShift()) {
      // Right shift (>>)
      if (dominant_type == OperandType::Byte) {
        EmitCommand("ByteRightShift");
      } else {
        EmitCommand("IntRightShift");
      }
    } else {
      EmitCommand("UnsupportedBinaryOp");
    }

    // If operation returns a primitive and result should be wrapped, wrap it
    // Comparison operations always return bool (primitive), so no wrapping
    if (!is_comparison) {
      // If both operands are wrappers of the same type, result should be that wrapper type
      if (IsPrimitiveWrapper(lhs_type_name) && lhs_type_name == rhs_type_name) {
        EmitWrapIfNeeded(lhs_type_name, dominant_type);
      }
      // If only LHS is wrapper, result should be that wrapper type
      else if (IsPrimitiveWrapper(lhs_type_name) && !IsPrimitiveWrapper(rhs_type_name)) {
        EmitWrapIfNeeded(lhs_type_name, dominant_type);
      }
      // If only RHS is wrapper, result should be that wrapper type
      else if (!IsPrimitiveWrapper(lhs_type_name) && IsPrimitiveWrapper(rhs_type_name)) {
        EmitWrapIfNeeded(rhs_type_name, dominant_type);
      }
    }
  }
}

void BytecodeVisitor::Visit(Unary& node) {
  const auto& op = node.Op();
  
  // For Unwrap operator (!), we don't need to unwrap primitive wrappers
  // This is for unwrapping nullable types, not primitive wrappers
  if (&op == &OpTags::Unwrap()) {
    node.MutableOperand().Accept(*this);
    // Unwrap nullable type - emit Unwrap command
    EmitCommand("Unwrap");
    return;
  }

  // Check if operand is a primitive wrapper and unwrap it if needed
  std::string operand_type_name = GetTypeNameForExpr(&node.MutableOperand());

  node.MutableOperand().Accept(*this);

  if (IsPrimitiveWrapper(operand_type_name)) {
    EmitCommand("Unwrap");
  }

  OperandType operand_type = DetermineOperandType(&node.MutableOperand());

  if (&op == &OpTags::Neg()) {
    if (operand_type == OperandType::Float) {
      EmitCommand("FloatNegate");
    } else if (operand_type == OperandType::Byte) {
      EmitCommand("ByteNegate");
    } else {
      EmitCommand("IntNegate");
    }
    // For negation, result type is same as operand type
    // If operand was a wrapper, wrap the result back
    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name, operand_type);
    }
  } else if (&op == &OpTags::Plus()) {
    // unary plus — no-op, but if operand was wrapper, result should be wrapper
    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name, operand_type);
    }
  } else if (&op == &OpTags::Not()) {
    // Logical NOT (!) - result is always bool (primitive)
    EmitCommand("BoolNot");
  } else if (&op == &OpTags::BitwiseNot()) {
    // Bitwise NOT (~) - result type is same as operand type
    if (operand_type == OperandType::Byte) {
      EmitCommand("ByteNot");
    } else {
      EmitCommand("IntNot");
    }
    // If operand was a wrapper, wrap the result back
    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name, operand_type);
    }
  } else {
    EmitCommand("UnsupportedUnaryOp");
  }
}

void BytecodeVisitor::Visit(Assign& node) {
  const auto& op = node.Kind();
  bool is_copy_assign = (&op == &OpTags::CopyAssign());

  // For SetField: we need [object, value] on stack
  // Strategy: push value first, then object, so stack is [object, value]

  // target
  if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableTarget())) {
    // For field assignment: this.x = value
    // We need: value, this, SetField index
    // Push value first
    node.MutableValue().Accept(*this);

    // For CopyAssign with reference types, perform deep copy
    if (is_copy_assign) {
      // Determine if value is a reference type
      // For now, assume all non-fundamental types are reference types
      // TODO: More precise type checking
      OperandType value_type = DetermineOperandType(&node.MutableValue());
      if (value_type == OperandType::String || value_type == OperandType::Unknown) {
        // Assume reference type - call Copy() method if available
        // For now, just use the value as-is (Copy semantics may be handled by runtime)
        // TODO: Emit Copy command or call Copy() method
      }
    }

    // Then push object (this)
    field_access->MutableObject().Accept(*this);
    // Stack now: [object, value] - correct for SetField

    // Find field index - try current class first, then search all classes
    int field_index = -1;
    std::string object_type_name;

    // Try to determine object type
    if (auto* ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
      auto type_it = variable_types_.find(ident->Name());
      if (type_it != variable_types_.end()) {
        object_type_name = type_it->second;
      }
    }

    // Search for field in object's class
    if (!object_type_name.empty()) {
      auto fields_it = class_fields_.find(object_type_name);
      if (fields_it != class_fields_.end()) {
        const auto& fields = fields_it->second;
        for (size_t i = 0; i < fields.size(); ++i) {
          if (fields[i].first == field_access->Name()) {
            field_index = static_cast<int>(i);
            break;
          }
        }
      }
    }

    // Fallback: search in current class
    if (field_index < 0 && !current_class_name_.empty()) {
      auto fields_it = class_fields_.find(current_class_name_);
      if (fields_it != class_fields_.end()) {
        const auto& fields = fields_it->second;
        for (size_t i = 0; i < fields.size(); ++i) {
          if (fields[i].first == field_access->Name()) {
            field_index = static_cast<int>(i);
            break;
          }
        }
      }
    }

    if (field_index < 0) {
      field_index = 0; // Fallback
    }
    EmitCommandWithInt("SetField", field_index);
    return;
  }

  // For CopyAssign with reference types (classes), call the copy method
  if (is_copy_assign) {
    // Determine types of target and value
    std::string target_type_name;
    std::string value_type_name;

    if (auto* target_ident = dynamic_cast<IdentRef*>(&node.MutableTarget())) {
      auto type_it = variable_types_.find(target_ident->Name());
      if (type_it != variable_types_.end()) {
        target_type_name = type_it->second;
      }
    }

    // Determine value type
    if (auto* value_ident = dynamic_cast<IdentRef*>(&node.MutableValue())) {
      auto type_it = variable_types_.find(value_ident->Name());
      if (type_it != variable_types_.end()) {
        value_type_name = type_it->second;
      }
    } else {
      value_type_name = GetTypeNameForExpr(&node.MutableValue());
    }

    // Check if both are reference types (classes, not fundamental types)
    bool target_is_ref = target_type_name != "int" && target_type_name != "float" && target_type_name != "byte" &&
                         target_type_name != "char" && target_type_name != "bool" && target_type_name != "pointer" &&
                         !target_type_name.empty();
    bool value_is_ref = value_type_name != "int" && value_type_name != "float" && value_type_name != "byte" &&
                        value_type_name != "char" && value_type_name != "bool" && value_type_name != "pointer" &&
                        !value_type_name.empty();

    // For CopyAssign with reference types, call the copy method: _{ClassName}_copy_<M>_{ClassName}
    // Method call format: arguments are pushed right-to-left first, then object (this)
    // For method calls, arguments are pushed first (right-to-left), then object
    // For a method with one argument: push arg first, then obj (this) → stack: [obj (this), arg]
    // Inside the method: LoadLocal 0 = this, LoadLocal 1 = arg
    if (target_is_ref && value_is_ref && target_type_name == value_type_name) {
      // Push value (argument) first - arguments are pushed right-to-left, for single arg just push it
      node.MutableValue().Accept(*this); // Push value (p2) - argument
      // Then push target (as this for method call) - object is pushed after arguments
      node.MutableTarget().Accept(*this); // Push target (p1) - this for method call

      // Stack now: [p1 (this), p2 (arg)] - correct for method call where this is at bottom, arg at top
      // Inside copy method: LoadLocal 0 = this (p1), LoadLocal 1 = arg (p2)
      // Generate copy method name: _{ClassName}_copy_<M>_{ClassName}
      std::string copy_method_name = GenerateCopyMethodId(target_type_name, value_type_name);
      EmitCommandWithStringWithoutBraces("Call", copy_method_name);
      return;
    }
  }

  // For regular assignment or CopyAssign with fundamental types: value first, then SetLocal
  node.MutableValue().Accept(*this);

  // target
  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableTarget())) {
    EmitCommandWithInt("SetLocal", static_cast<int64_t>(GetLocalIndex(ident->Name())));
  } else if (auto* index_access = dynamic_cast<IndexAccess*>(&node.MutableTarget())) {
    // For SetIndex: we need [object, index, value]
    // Currently: value is on top
    // Push object, then index (right-to-left for SetIndex: value is already pushed, then index, then object)
    index_access->MutableIndexExpr().Accept(*this);
    index_access->MutableObject().Accept(*this);
    // Stack now: [object, index, value] - correct for SetIndex (object at bottom, value at top)
    EmitCommand("SetIndex");
  } else {
    EmitCommand("UnsupportedAssignTarget");
  }
}

void BytecodeVisitor::Visit(Call& node) {
  // Push arguments right-to-left
  auto& args = node.MutableArgs();
  for (auto it = args.rbegin(); it != args.rend(); ++it) {
    (*it)->Accept(*this);
  }

  // If callee is NamespaceRef (sys::PrintLine), handle built-in commands
  if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&node.MutableCallee())) {
    std::string ns_name = ns_ref->Name();
    // Handle sys:: namespace functions as built-in commands
    // TODO: Consider loading this list from a configuration file or making it extensible
    // TODO: Verify all these commands are actually supported by the bytecode VM
    static const std::unordered_set<std::string> kBuiltinCommands = {"Print",
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
                                                                     "GetEnvironmentVariable",
                                                                     "SetEnvironmentVariable",
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
                                                                     "GetLastError",
                                                                     "ClearError",
                                                                     "Interop",
                                                                     "TypeOf"};

    if (kBuiltinCommands.find(ns_name) != kBuiltinCommands.end()) {
      EmitCommand(ns_name);
      return;
    }
    // For other namespace functions, try to find mangled name
    std::string full_name = "sys::" + ns_name;
    auto it = function_name_map_.find(full_name);
    if (it != function_name_map_.end()) {
      EmitCommandWithStringWithoutBraces("Call", it->second);
      return;
    }
    // fallback
    EmitCommandWithStringWithoutBraces("Call", full_name);
    return;
  }

  // If callee is a simple identifier
  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableCallee())) {
    std::string name = ident->Name();

    // Check if it's a constructor call (type name like Int, Float, String, etc.)
    // Common built-in type constructors
    static const std::unordered_set<std::string> builtin_types = {"Int",
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
                                                                  "ObjectArray"};

    if (builtin_types.find(name) != builtin_types.end()) {
      // Generate constructor name: _TypeName_paramTypes
      std::string constructor_name = "_" + name;
      if (args.size() == 1) {
        // For single argument constructors, determine type from constructor name
        std::string param_type;
        if (name == "Int")
          param_type = "int";
        else if (name == "Float")
          param_type = "float";
        else if (name == "String")
          param_type = "String";
        else if (name == "Bool")
          param_type = "bool";
        else if (name == "Char")
          param_type = "char";
        else if (name == "Byte")
          param_type = "byte";
        else
          param_type = "Object"; // fallback
        constructor_name += "_" + param_type;
      } else if (args.size() == 2) {
        // For two arguments (typically arrays: size, initial_value)
        std::string type1 = "int"; // size
        std::string type2;
        if (name == "IntArray")
          type2 = "int";
        else if (name == "FloatArray")
          type2 = "float";
        else if (name == "StringArray")
          type2 = "String";
        else if (name == "BoolArray")
          type2 = "bool";
        else if (name == "ByteArray")
          type2 = "byte";
        else if (name == "CharArray")
          type2 = "char";
        else if (name == "ObjectArray")
          type2 = "Object";
        else
          type2 = "Object"; // fallback
        constructor_name += "_" + type1 + "_" + type2;
      } else if (args.size() == 3) {
        // For three arguments (e.g., Rectangle: width, height)
        constructor_name += "_float_float"; // Common case
      }
      EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
      return;
    }

    // Check if it's a constructor call for a user-defined class
    // Look for registered constructor in method_name_map_ with key "ClassName::<ctor>"
    std::string ctor_key = name + "::<ctor>";
    auto ctor_it = method_name_map_.find(ctor_key);
    if (ctor_it != method_name_map_.end()) {
      // This is a constructor call for a user-defined class
      EmitCommandWithStringWithoutBraces("CallConstructor", ctor_it->second);
      // After CallConstructor, we need to get and set the vtable for the object
      // GetVTable gets the vtable for the class and pushes it onto the stack
      // The object is already on the stack from CallConstructor
      EmitGetVTable(name);
      return;
    }

    // Check if it's a registered function
    auto it = function_name_map_.find(name);
    if (it != function_name_map_.end()) {
      EmitCommandWithStringWithoutBraces("Call", it->second);
      return;
    }
    // fallback to name as-is
    EmitCommandWithStringWithoutBraces("Call", name);
    return;
  }

  // If callee is FieldAccess (a.method), call method
  if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableCallee())) {
    // emit object first
    field_access->MutableObject().Accept(*this);

    std::string method_name = field_access->Name();

    // Try to determine object type
    std::string object_type;
    if (auto* ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
      // Object is a variable - try to get its type
      auto type_it = variable_types_.find(ident->Name());
      if (type_it != variable_types_.end()) {
        object_type = type_it->second;
      }
    } else if (auto* string_lit = dynamic_cast<StringLit*>(&field_access->MutableObject())) {
      object_type = "String";
    } else if (auto* int_lit = dynamic_cast<IntLit*>(&field_access->MutableObject())) {
      object_type = "int";
    } else if (auto* float_lit = dynamic_cast<FloatLit*>(&field_access->MutableObject())) {
      object_type = "float";
    }

    // Check if method exists in multiple classes (virtual method case)
    // Also check if calling through interface (interfaces always use virtual dispatch)
    // Count how many classes have a method with this name
    int method_count = 0;
    std::string full_method_name;
    std::string vtable_name;
    
    // Check if object_type is an interface (interfaces start with 'I' by convention)
    // If type name starts with 'I' followed by uppercase letter and method not found in method_name_map_,
    // it's likely an interface type (interfaces don't have method implementations in method_name_map_)
    bool is_interface_type = false;
    if (!object_type.empty() && object_type.size() > 1 && object_type[0] == 'I') {
      if (std::isupper(static_cast<unsigned char>(object_type[1]))) {
        // Check if method not found for this type (interfaces don't have implementations)
        if (method_name_map_.find(object_type + "::" + method_name) == method_name_map_.end()) {
          is_interface_type = true;
        }
      }
    }
    
    for (const auto& kv : method_name_map_) {
      auto pos = kv.first.rfind("::");
      if (pos != std::string::npos) {
        std::string mname = kv.first.substr(pos + 2);
        if (mname == method_name) {
          method_count++;
          if (full_method_name.empty()) {
            full_method_name = kv.second;
          }
          // Get vtable name for this method (should be same for all classes)
          auto vtable_it = method_vtable_map_.find(kv.first);
          if (vtable_it != method_vtable_map_.end() && vtable_name.empty()) {
            vtable_name = vtable_it->second;
          }
        }
      }
    }

    // Use CallVirtual if:
    // 1. Method found in multiple classes (virtual dispatch)
    // 2. Calling through interface type (interfaces always use virtual dispatch)
    // 3. Method not found in object's type but exists in other classes (polymorphism)
    bool use_virtual = (method_count > 1) || 
                       is_interface_type ||
                       (!object_type.empty() && method_count > 0 && 
                        method_name_map_.find(object_type + "::" + method_name) == method_name_map_.end());
    
    if (use_virtual) {
      // Use CallVirtual for virtual method dispatch
      if (vtable_name.empty()) {
        // Generate vtable name: _MethodName_<C> or _MethodName_<C>_ParamTypes
        // For methods with no parameters, it's just _MethodName_<C>
        bool is_mutable = false; // TODO: determine from method signature
        vtable_name = GenerateMethodVTableName(method_name, std::vector<Param>(), is_mutable);
      }
      EmitCommandWithStringWithoutBraces("CallVirtual", vtable_name);
    } else if (!full_method_name.empty()) {
      // Direct call for non-virtual methods (found in single class)
      EmitCommandWithStringWithoutBraces("Call", full_method_name);
    } else {
      // Fallback: try common built-in types
      // Common methods on built-in types
      static const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> builtin_methods = {
          {"String",
           {
               {"GetHash", "_String_GetHash_<C>"},
               {"ToString", "_String_ToString_<C>"},
               {"Length", "_String_Length_<C>"},
               {"Equals", "_String_Equals_<C>_Object"},
               {"Substring", "_String_Substring_<C>_int_int"},
               {"Compare", "_String_Compare_<C>_String"},
           }},
          {"Int",
           {
               {"ToString", "_Int_ToString_<C>"},
               {"GetHash", "_Int_GetHash_<C>"},
               {"Equals", "_Int_Equals_<C>_Object"},
           }},
          {"Float",
           {
               {"ToString", "_Float_ToString_<C>"},
               {"GetHash", "_Float_GetHash_<C>"},
               {"Equals", "_Float_Equals_<C>_Object"},
           }},
          {"int",
           {
               {"ToString", "_Int_ToString_<C>"},
               {"GetHash", "_Int_GetHash_<C>"},
           }},
          {"float",
           {
               {"ToString", "_Float_ToString_<C>"},
               {"GetHash", "_Float_GetHash_<C>"},
           }},
      };

      std::string method_call;
      if (!object_type.empty()) {
        auto type_it = builtin_methods.find(object_type);
        if (type_it != builtin_methods.end()) {
          auto method_it = type_it->second.find(method_name);
          if (method_it != type_it->second.end()) {
            method_call = method_it->second;
          }
        }
      }

      if (!method_call.empty()) {
        EmitCommandWithStringWithoutBraces("Call", method_call);
      } else {
        // Unknown method - try CallVirtual as fallback
        std::string vtable_name = "_" + method_name + "_<C>";
        if (!args.empty()) {
          vtable_name += "_Object"; // Simplified
        }
        EmitCommandWithStringWithoutBraces("CallVirtual", vtable_name);
      }
    }
    return;
  }

  // generic fallback: evaluate callee then CallDynamic
  node.MutableCallee().Accept(*this);
  EmitCommand("CallDynamic");
}

void BytecodeVisitor::Visit(FieldAccess& node) {
  node.MutableObject().Accept(*this);

  // Find field index - try to determine object type first
  int field_index = -1;
  std::string object_type_name;

  // Try to determine object type from variable
  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableObject())) {
    auto type_it = variable_types_.find(ident->Name());
    if (type_it != variable_types_.end()) {
      object_type_name = type_it->second;
    }
  }

  // Search for field in object's class and determine field type
  std::string field_type_name;
  if (!object_type_name.empty()) {
    auto fields_it = class_fields_.find(object_type_name);
    if (fields_it != class_fields_.end()) {
      const auto& fields = fields_it->second;
      for (size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].first == node.Name()) {
          field_index = static_cast<int>(i);
          field_type_name = TypeToMangledName(fields[i].second);
          break;
        }
      }
    }
  }

  // Fallback: search in current class
  if (field_index < 0 && !current_class_name_.empty()) {
    auto fields_it = class_fields_.find(current_class_name_);
    if (fields_it != class_fields_.end()) {
      const auto& fields = fields_it->second;
      for (size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].first == node.Name()) {
          field_index = static_cast<int>(i);
          if (field_type_name.empty()) {
            field_type_name = TypeToMangledName(fields[i].second);
          }
          break;
        }
      }
    }
  }

  if (field_index < 0) {
    field_index = 0; // Fallback
  }

  EmitCommandWithInt("GetField", field_index);

  // Automatically emit Unwrap for primitive wrapper types if field type is a wrapper
  if (!field_type_name.empty()) {
    EmitUnwrapIfNeeded(field_type_name);
  }
}

void BytecodeVisitor::Visit(IndexAccess& node) {
  node.MutableObject().Accept(*this);
  node.MutableIndexExpr().Accept(*this);
  EmitCommand("GetIndex");
}

void BytecodeVisitor::Visit(NamespaceRef& node) {
  // treat namespace references as static variable load
  EmitCommandWithInt("LoadStatic", static_cast<int64_t>(GetStaticIndex(node.Name())));
}

void BytecodeVisitor::Visit(SafeCall& node) {
  node.MutableObject().Accept(*this);
  for (auto it = node.MutableArgs().rbegin(); it != node.MutableArgs().rend(); ++it) {
    (*it)->Accept(*this);
  }
  EmitCommandWithStringWithoutBraces("SafeCall", node.Method());
}

void BytecodeVisitor::Visit(Elvis& node) {
  // lhs ?: rhs
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
  EmitCommand("NullCoalesce");
}

void BytecodeVisitor::Visit(CastAs& node) {
  node.MutableExpression().Accept(*this);

  // Determine source and target types
  OperandType source_type = DetermineOperandType(&node.MutableExpression());
  std::string target_type_name = TypeToMangledName(node.Type());

  // Emit type conversion commands for common conversions
  if (source_type == OperandType::Int && (target_type_name == "float" || target_type_name == "Float")) {
    EmitCommand("IntToFloat");
  } else if (source_type == OperandType::Float && (target_type_name == "int" || target_type_name == "Int")) {
    EmitCommand("FloatToInt");
  } else if (source_type == OperandType::Byte && (target_type_name == "int" || target_type_name == "Int")) {
    EmitCommand("ByteToInt");
  } else if (source_type == OperandType::Int && (target_type_name == "byte" || target_type_name == "Byte")) {
    // Int to Byte - use IntToByte if available, otherwise use CastAs
    EmitCommandWithStringWithoutBraces("CastAs", target_type_name);
  } else if (source_type == OperandType::String && (target_type_name == "int" || target_type_name == "Int")) {
    EmitCommand("StringToInt");
  } else if (source_type == OperandType::String && (target_type_name == "float" || target_type_name == "Float")) {
    EmitCommand("StringToFloat");
  } else if (source_type == OperandType::Int && target_type_name == "String") {
    EmitCommand("IntToString");
  } else if (source_type == OperandType::Float && target_type_name == "String") {
    EmitCommand("FloatToString");
  } else if (source_type == OperandType::Char && (target_type_name == "byte" || target_type_name == "Byte")) {
    EmitCommand("CharToByte");
  } else if (source_type == OperandType::Byte && (target_type_name == "char" || target_type_name == "Char")) {
    EmitCommand("ByteToChar");
  } else if (source_type == OperandType::Bool && (target_type_name == "byte" || target_type_name == "Byte")) {
    EmitCommand("BoolToByte");
  } else {
    // Generic cast for other types
    EmitCommandWithStringWithoutBraces("CastAs", target_type_name);
  }
}

void BytecodeVisitor::Visit(TypeTestIs& node) {
  node.MutableExpression().Accept(*this);
  std::string type_name = TypeToMangledName(node.Type());
  EmitCommandWithStringWithoutBraces("IsType", type_name);
}

void BytecodeVisitor::Visit(IdentRef& node) {
  // Prefer local; fallback to static/global
  if (local_variables_.contains(node.Name())) {
    EmitCommandWithInt("LoadLocal", static_cast<int64_t>(GetLocalIndex(node.Name())));
  } else {
    EmitCommandWithInt("LoadStatic", static_cast<int64_t>(GetStaticIndex(node.Name())));
  }
}

void BytecodeVisitor::Visit(IntLit& node) {
  EmitCommandWithInt("PushInt", node.Value());
}

void BytecodeVisitor::Visit(FloatLit& node) {
  EmitCommandWithFloat("PushFloat", node.Value());
}

void BytecodeVisitor::Visit(StringLit& node) {
  EmitCommandWithString("PushString", node.Value());
}

void BytecodeVisitor::Visit(CharLit& node) {
  EmitCommandWithInt("PushChar", static_cast<int64_t>(node.Value()));
}

void BytecodeVisitor::Visit(BoolLit& node) {
  EmitCommandWithBool("PushBool", node.Value());
}

void BytecodeVisitor::Visit(ByteLit& node) {
  EmitPushByte(node.Value());
}

void BytecodeVisitor::Visit(NullLit& node) {
  EmitCommand("PushNull");
}

void BytecodeVisitor::Visit(ThisExpr& node) {
  // 'this' is always at local index 0 in methods and constructors
  EmitCommandWithInt("LoadLocal", 0);
}

std::string BytecodeVisitor::GenerateFunctionId(const std::string& name, const std::vector<Param>& params) {
  std::ostringstream oss;
  oss << "_Global_" << name;
  if (!params.empty()) {
    oss << "_";
    for (size_t i = 0; i < params.size(); ++i) {
      if (i) {
        oss << "_";
      }
      oss << TypeToMangledName(params[i].GetType());
    }
  }
  return oss.str();
}

std::string BytecodeVisitor::GenerateMethodId(const std::string& class_name,
                                              const std::string& method_name,
                                              const std::vector<Param>& params,
                                              bool is_constructor,
                                              bool is_destructor,
                                              bool is_mutable) {
  std::ostringstream oss;
  if (is_constructor) {
    // Constructor: _ClassName_param1Type_param2Type
    oss << "_" << class_name;
    if (!params.empty()) {
      oss << "_";
      for (size_t i = 0; i < params.size(); ++i) {
        if (i) {
          oss << "_";
        }
        oss << TypeToMangledName(params[i].GetType());
      }
    }
  } else if (is_destructor) {
    oss << "_" << class_name << "_destructor";
  } else {
    // Method: _ClassName_methodName_<C>_param1Type_param2Type or _ClassName_methodName_<M>_param1Type_param2Type
    oss << "_" << class_name << "_" << method_name;
    if (is_mutable) {
      oss << "_<M>";
    } else {
      oss << "_<C>";
    }
    if (!params.empty()) {
      oss << "_";
      for (size_t i = 0; i < params.size(); ++i) {
        if (i) {
          oss << "_";
        }
        oss << TypeToMangledName(params[i].GetType());
      }
    }
  }
  return oss.str();
}

std::string BytecodeVisitor::GenerateConstructorId(const std::string& class_name, const std::vector<Param>& params) {
  std::ostringstream oss;
  oss << "_" << class_name;
  if (!params.empty()) {
    oss << "_";
    for (size_t i = 0; i < params.size(); ++i) {
      if (i) {
        oss << "_";
      }
      oss << TypeToMangledName(params[i].GetType());
    }
  }
  return oss.str();
}

std::string BytecodeVisitor::GenerateDestructorId(const std::string& class_name) {
  return "_" + class_name + "_destructor";
}

std::string BytecodeVisitor::GenerateCopyMethodId(const std::string& class_name, const std::string& param_type) {
  // Format: _{ClassName}_copy_<M>_{ClassName}
  // This is a special member method that can only be called via := operator
  return "_" + class_name + "_copy_<M>_" + param_type;
}

std::string BytecodeVisitor::GenerateMethodVTableName(const std::string& method_name,
                                                      const std::vector<Param>& params,
                                                      bool is_mutable) {
  std::ostringstream oss;
  // Format: _methodName_<C>_param1Type_param2Type or _methodName_<M>_param1Type_param2Type
  oss << "_" << method_name;
  if (is_mutable) {
    oss << "_<M>";
  } else {
    oss << "_<C>";
  }
  if (!params.empty()) {
    oss << "_";
    for (size_t i = 0; i < params.size(); ++i) {
      if (i) {
        oss << "_";
      }
      oss << TypeToMangledName(params[i].GetType());
    }
  }
  return oss.str();
}

std::string BytecodeVisitor::TypeToMangledName(const TypeReference& type) {
  std::ostringstream oss;
  const auto& qname = type.QualifiedName();
  // For mangled names, use only the simple name (last component), not the full qualified name
  if (!qname.empty()) {
    oss << qname.back(); // Use last component only
  }

  if (type.Arity() > 0) {
    oss << "<";
    for (size_t i = 0; i < type.Arity(); ++i) {
      if (i) {
        oss << ",";
      }
      oss << TypeToMangledName(type.TypeArguments()[i]);
    }
    oss << ">";
  }

  if (type.IsNullable()) {
    oss << "?";
  }

  return oss.str();
}

void BytecodeVisitor::VisitExpression(Expr* expr) {
  if (expr != nullptr) {
    expr->Accept(*this);
  }
}

void BytecodeVisitor::VisitStatement(Stmt* stmt) {
  if (stmt != nullptr) {
    stmt->Accept(*this);
  }
}

void BytecodeVisitor::VisitBlock(Block* block) {
  if (block != nullptr) {
    block->Accept(*this);
  }
}

size_t BytecodeVisitor::GetLocalIndex(const std::string& name) {
  if (local_variables_.find(name) == local_variables_.end()) {
    local_variables_[name] = next_local_index_++;
  }
  return local_variables_[name];
}

size_t BytecodeVisitor::GetStaticIndex(const std::string& name) {
  if (static_variables_.find(name) == static_variables_.end()) {
    static_variables_[name] = next_static_index_++;
  }
  return static_variables_[name];
}

void BytecodeVisitor::ResetLocalVariables() {
  local_variables_.clear();
  variable_types_.clear(); // Also clear variable types
  next_local_index_ = 0;
}

// Determines the operand type of an expression for proper bytecode generation
// Uses TypeReference information from AST nodes instead of just dynamic_cast on literals
// This allows correct type-aware code generation for variables, fields, and complex expressions
BytecodeVisitor::OperandType BytecodeVisitor::DetermineOperandType(Expr* expr) {
  if (expr == nullptr) {
    return OperandType::Unknown;
  }

  // Check literals first (fast path for constant values)
  if (dynamic_cast<IntLit*>(expr) != nullptr) {
    return OperandType::Int;
  }
  if (dynamic_cast<FloatLit*>(expr) != nullptr) {
    return OperandType::Float;
  }
  if (dynamic_cast<StringLit*>(expr) != nullptr) {
    return OperandType::String;
  }
  if (dynamic_cast<BoolLit*>(expr) != nullptr) {
    return OperandType::Bool;
  }
  if (dynamic_cast<CharLit*>(expr) != nullptr) {
    return OperandType::Char;
  }

  // Check variable types from variable_types_ map
  if (auto* ident = dynamic_cast<IdentRef*>(expr)) {
    auto it = variable_types_.find(ident->Name());
    if (it != variable_types_.end()) {
      const std::string& type_name = it->second;
      if (type_name == "int" || type_name == "Int") {
        return OperandType::Int;
      } else if (type_name == "float" || type_name == "Float") {
        return OperandType::Float;
      } else if (type_name == "byte" || type_name == "Byte") {
        return OperandType::Byte;
      } else if (type_name == "bool" || type_name == "Bool") {
        return OperandType::Bool;
      } else if (type_name == "char" || type_name == "Char") {
        return OperandType::Char;
      } else if (type_name == "String") {
        return OperandType::String;
      }
    }
  }

  // Check FieldAccess - try to get type from field declaration in class_fields_
  // Searches in object's class first, then falls back to current class
  if (auto* field_access = dynamic_cast<FieldAccess*>(expr)) {
    if (!current_class_name_.empty()) {
      auto fields_it = class_fields_.find(current_class_name_);
      if (fields_it != class_fields_.end()) {
        const auto& fields = fields_it->second;
        for (const auto& field : fields) {
          if (field.first == field_access->Name()) {
            const std::string& type_name = TypeToMangledName(field.second);
            if (type_name == "int" || type_name == "Int") {
              return OperandType::Int;
            } else if (type_name == "float" || type_name == "Float") {
              return OperandType::Float;
            } else if (type_name == "byte" || type_name == "Byte") {
              return OperandType::Byte;
            } else if (type_name == "bool" || type_name == "Bool") {
              return OperandType::Bool;
            } else if (type_name == "char" || type_name == "Char") {
              return OperandType::Char;
            } else if (type_name == "String") {
              return OperandType::String;
            }
            break;
          }
        }
      }
    }
  }

  // Check CastAs - use the target type (cast expression's result type)
  if (auto* cast = dynamic_cast<CastAs*>(expr)) {
    const std::string& type_name = TypeToMangledName(cast->Type());
    if (type_name == "int" || type_name == "Int") {
      return OperandType::Int;
    } else if (type_name == "float" || type_name == "Float") {
      return OperandType::Float;
    } else if (type_name == "byte" || type_name == "Byte") {
      return OperandType::Byte;
    } else if (type_name == "bool" || type_name == "Bool") {
      return OperandType::Bool;
    } else if (type_name == "char" || type_name == "Char") {
      return OperandType::Char;
    } else if (type_name == "String") {
      return OperandType::String;
    }
  }

  // Check Binary/Unary - recursively determine from operands
  // For binary operations, returns the dominant type (Float > Int > Byte > String > Bool)
  if (auto* binary = dynamic_cast<Binary*>(expr)) {
    OperandType lhs_type = DetermineOperandType(&binary->MutableLhs());
    OperandType rhs_type = DetermineOperandType(&binary->MutableRhs());

    // For shift operations (<<, >>), result type is the same as left operand type
    const auto& op = binary->Op();
    if (&op == &OpTags::LeftShift() || &op == &OpTags::RightShift()) {
      return lhs_type != OperandType::Unknown ? lhs_type : OperandType::Int;
    }

    // For bitwise operations (and, or, xor), if one operand is Byte, prefer Byte
    bool is_bitwise = (&op == &OpTags::BitwiseAnd() || &op == &OpTags::BitwiseOr() || &op == &OpTags::Xor());
    if (is_bitwise && (lhs_type == OperandType::Byte || rhs_type == OperandType::Byte)) {
      return OperandType::Byte;
    }

    // Return dominant type based on type promotion rules
    if (lhs_type == OperandType::Float || rhs_type == OperandType::Float) {
      return OperandType::Float;
    } else if (lhs_type == OperandType::Int || rhs_type == OperandType::Int) {
      return OperandType::Int;
    } else if (lhs_type == OperandType::Byte || rhs_type == OperandType::Byte) {
      return OperandType::Byte;
    } else if (lhs_type == OperandType::String || rhs_type == OperandType::String) {
      return OperandType::String;
    } else if (lhs_type != OperandType::Unknown) {
      return lhs_type;
    } else if (rhs_type != OperandType::Unknown) {
      return rhs_type;
    }
  }

  // For unary operations, return the type of the operand
  if (auto* unary = dynamic_cast<Unary*>(expr)) {
    return DetermineOperandType(&unary->MutableOperand());
  }

  return OperandType::Unknown;
}

std::string BytecodeVisitor::GetOperandTypeName(Expr* expr) {
  OperandType type = DetermineOperandType(expr);
  switch (type) {
    case OperandType::Int:
      return "int";
    case OperandType::Float:
      return "float";
    case OperandType::Byte:
      return "byte";
    case OperandType::Bool:
      return "bool";
    case OperandType::String:
      return "String";
    default:
      return "unknown";
  }
}

std::string BytecodeVisitor::GetTypeNameForExpr(Expr* expr) {
  if (expr == nullptr) {
    return "unknown";
  }

  // Check if it's a variable reference
  if (auto* ident = dynamic_cast<IdentRef*>(expr)) {
    auto it = variable_types_.find(ident->Name());
    if (it != variable_types_.end()) {
      return it->second;
    }
  }

  // Check if it's a field access
  if (auto* field = dynamic_cast<FieldAccess*>(expr)) {
    // Try to determine type from field declaration
    if (!current_class_name_.empty()) {
      auto fields_it = class_fields_.find(current_class_name_);
      if (fields_it != class_fields_.end()) {
        for (const auto& f : fields_it->second) {
          if (f.first == field->Name()) {
            return TypeToMangledName(f.second);
          }
        }
      }
    }
  }

  // For literals, return primitive type (they are always primitives)
  if (dynamic_cast<IntLit*>(expr) != nullptr)
    return "int";
  if (dynamic_cast<FloatLit*>(expr) != nullptr)
    return "float";
  if (dynamic_cast<ByteLit*>(expr) != nullptr)
    return "byte";
  if (dynamic_cast<CharLit*>(expr) != nullptr)
    return "char";
  if (dynamic_cast<BoolLit*>(expr) != nullptr)
    return "bool";
  if (dynamic_cast<StringLit*>(expr) != nullptr)
    return "String";

  // For other expressions, return primitive type based on OperandType
  return GetOperandTypeName(expr);
}

bool BytecodeVisitor::IsPrimitiveWrapper(const std::string& type_name) const {
  return type_name == "Int" || type_name == "Float" || type_name == "Byte" || type_name == "Char" ||
         type_name == "Bool";
}

bool BytecodeVisitor::IsPrimitiveType(const std::string& type_name) const {
  return type_name == "int" || type_name == "float" || type_name == "byte" || type_name == "char" ||
         type_name == "bool";
}

std::string BytecodeVisitor::GetPrimitiveTypeForWrapper(const std::string& wrapper_type) const {
  if (wrapper_type == "Int")
    return "int";
  if (wrapper_type == "Float")
    return "float";
  if (wrapper_type == "Byte")
    return "byte";
  if (wrapper_type == "Char")
    return "char";
  if (wrapper_type == "Bool")
    return "bool";
  return wrapper_type;
}

std::string BytecodeVisitor::GetWrapperTypeForPrimitive(const std::string& primitive_type) const {
  if (primitive_type == "int")
    return "Int";
  if (primitive_type == "float")
    return "Float";
  if (primitive_type == "byte")
    return "Byte";
  if (primitive_type == "char")
    return "Char";
  if (primitive_type == "bool")
    return "Bool";
  return primitive_type;
}

void BytecodeVisitor::EmitUnwrapIfNeeded(const std::string& type_name) {
  if (IsPrimitiveWrapper(type_name)) {
    EmitCommand("Unwrap");
  }
}

void BytecodeVisitor::EmitWrapIfNeeded(const std::string& expected_type, OperandType result_type) {
  // If expected type is a wrapper and result is primitive, wrap it
  if (IsPrimitiveWrapper(expected_type)) {
    std::string primitive_type = GetPrimitiveTypeForWrapper(expected_type);
    // Constructor name format: _WrapperType_primitiveType (e.g., _Int_int)
    std::string constructor_name = "_" + expected_type + "_" + primitive_type;
    EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
  }
}

} // namespace ovum::compiler::parser
