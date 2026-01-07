#include "BytecodeVisitor.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
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
// helper: simple size heuristic for field layout (matches examples: int/float/object -> 8)
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

void BytecodeVisitor::EmitBlockStart() {
  EmitIndent();
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
  function_name_map_.clear();
  method_name_map_.clear();
  class_fields_.clear();
  pending_init_static_.clear();

  // scan declarations to register names and collect static inits
  for (auto& decl : node.MutableDecls()) {
    // function declarations
    if (auto* f = dynamic_cast<FunctionDecl*>(decl.get())) {
      std::string mangled = GenerateFunctionId(f->Name(), f->Params());
      function_name_map_[f->Name()] = mangled;
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
          std::string mid = GenerateMethodId(class_name, md->Name(), md->Params(), false, false);
          method_name_map_[class_name + "::" + md->Name()] = mid;
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
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());
  }

  // register in function map (if not already)
  std::string mangled = GenerateFunctionId(node.Name(), node.Params());

  // Emit function declaration
  output_ << "function:" << node.Params().size() << " " << mangled << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(MethodDecl& node) {
  ResetLocalVariables();
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());
  }

  std::string method_id = GenerateMethodId(current_class_name_, node.Name(), node.Params(), false, false);
  // register mapping class::name -> mangled
  method_name_map_[current_class_name_ + "::" + node.Name()] = method_id;

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
  for (auto& param : node.Params()) {
    GetLocalIndex(param.GetName());
  }

  std::string call_id = GenerateConstructorId(current_class_name_, node.Params());
  // store constructor mapping as Class::<ctor>
  method_name_map_[current_class_name_ + "::<ctor>"] = call_id;

  output_ << "function:" << node.Params().size() << " " << call_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
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
  std::string id = GenerateMethodId(
      /* class_name */ "",
      node.Name(),
      node.MutableParams(),
      /* is_constructor */ false,
      /* is_destructor */ false);

  method_name_map_[node.Name()] = id;
}

void BytecodeVisitor::Visit(TypeAliasDecl&) {
}

void BytecodeVisitor::Visit(GlobalVarDecl& node) {
  std::size_t index = GetStaticIndex(node.Name());

  if (node.MutableInit()) {
    node.MutableInit()->Accept(*this);
    EmitCommandWithInt("SetStatic", static_cast<int64_t>(index));
  }
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

  std::string destructor_id = GenerateDestructorId(current_class_name_);
  method_name_map_[current_class_name_ + "::<dtor>"] = destructor_id;

  output_ << "function:0 " << destructor_id << " ";
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
      std::string mid = GenerateMethodId(node.Name(), md->Name(), md->Params(), false, false);
      method_map.emplace_back(md->Name(), mid);
      method_name_map_[node.Name() + "::" + md->Name()] = mid;
    }
    if (auto* cd = dynamic_cast<CallDecl*>(member.get())) {
      std::string ce = GenerateConstructorId(node.Name(), cd->Params());
      method_map.emplace_back(std::string("constructor"), ce);
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

  // methods
  EmitIndent();
  output_ << "methods {\n";
  indent_level_++;
  for (auto& m : method_map) {
    EmitIndent();
    output_ << m.first << ": " << m.second << "\n";
  }
  indent_level_--;
  EmitIndent();
  output_ << "}\n";

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
}

void BytecodeVisitor::Visit(ExprStmt& node) {
  if (node.MutableExpression() != nullptr) {
    node.MutableExpression()->Accept(*this);
  }
  // expression statement result must be popped
  EmitCommand("Pop");
}

void BytecodeVisitor::Visit(ReturnStmt& node) {
  if (node.MutableValue() != nullptr) {
    node.MutableValue()->Accept(*this);
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
  EmitBlockStart();
  if (node.MutableCondition() != nullptr) {
    node.MutableCondition()->Accept(*this);
  }
  EmitBlockEnd();

  EmitIndent();
  output_ << "then ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
}

void BytecodeVisitor::Visit(ForStmt& node) {
  if (node.MutableIteratorExpr() != nullptr) {
    node.MutableIteratorExpr()->Accept(*this);
  }
  // TODO: Lower for -> while
}

void BytecodeVisitor::Visit(UnsafeBlock& node) {
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
}

void BytecodeVisitor::Visit(Binary& node) {
  // push rhs then lhs (right-to-left)
  node.MutableRhs().Accept(*this);
  node.MutableLhs().Accept(*this);

  const auto& op = node.Op();

  if (&op == &OpTags::Add()) {
    EmitCommand("IntAdd");
  } else if (&op == &OpTags::Sub()) {
    EmitCommand("IntSubtract");
  } else if (&op == &OpTags::Mul()) {
    EmitCommand("IntMultiply");
  } else if (&op == &OpTags::Div()) {
    EmitCommand("IntDivide");
  } else if (&op == &OpTags::Mod()) {
    EmitCommand("IntModulo");
  } else if (&op == &OpTags::Lt()) {
    EmitCommand("IntLessThan");
  } else if (&op == &OpTags::Le()) {
    EmitCommand("IntLessEqual");
  } else if (&op == &OpTags::Gt()) {
    EmitCommand("IntGreaterThan");
  } else if (&op == &OpTags::Ge()) {
    EmitCommand("IntGreaterEqual");
  } else if (&op == &OpTags::Eq()) {
    EmitCommand("IntEqual");
  } else if (&op == &OpTags::Ne()) {
    EmitCommand("IntNotEqual");
  } else if (&op == &OpTags::And()) {
    EmitCommand("BoolAnd");
  } else if (&op == &OpTags::Or()) {
    EmitCommand("BoolOr");
  } else if (&op == &OpTags::Xor()) {
    EmitCommand("IntXor");
  } else {
    EmitCommand("UnsupportedBinaryOp");
  }
}

void BytecodeVisitor::Visit(Unary& node) {
  node.MutableOperand().Accept(*this);

  const auto& op = node.Op();

  if (&op == &OpTags::Neg()) {
    EmitCommand("IntNegate");
  } else if (&op == &OpTags::Plus()) {
    // unary plus — no-op
  } else if (&op == &OpTags::Not()) {
    EmitCommand("BoolNot");
  } else {
    EmitCommand("UnsupportedUnaryOp");
  }
}

void BytecodeVisitor::Visit(Assign& node) {
  // value first
  node.MutableValue().Accept(*this);

  const auto& op = node.Kind();

  if (&op == &OpTags::CopyAssign()) {
    // currently no special instructions — emit 'Copy' placeholder if required
    EmitCommand("Copy");
  }

  // target
  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableTarget())) {
    EmitCommandWithInt("SetLocal", static_cast<int64_t>(GetLocalIndex(ident->Name())));
  } else if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableTarget())) {
    // object, value already on stack? ensure order: value is on top currently,
    // we need object beneath it. So emit object first then Swap (to have object, value).
    field_access->MutableObject().Accept(*this);
    // Stack now: object, value -> need order: object, value for SetField
    // But we currently have value (from earlier) above object; simpler: after object accept,
    // emit Swap to put object under value if required.
    EmitCommand("Swap");
    // placeholder field index resolution
    int field_index = 0;
    EmitCommandWithInt("SetField", field_index);
  } else if (auto* index_access = dynamic_cast<IndexAccess*>(&node.MutableTarget())) {
    index_access->MutableObject().Accept(*this);
    index_access->MutableIndexExpr().Accept(*this);
    // order: object, index, value expected -> we currently have value on top; rotate to place value last
    // We will emit SetIndex (placeholder)
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

  // If callee is simple identifier and we have a registered mangled name, use it.
  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableCallee())) {
    auto it = function_name_map_.find(ident->Name());
    if (it != function_name_map_.end()) {
      EmitCommandWithString("Call", it->second);
      return;
    }
    // fallback to name as-is
    EmitCommandWithString("Call", ident->Name());
    return;
  }

  // If callee is FieldAccess (a.method), call virtual
  if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableCallee())) {
    // emit object, then call virtual method by method name or mangled if available
    field_access->MutableObject().Accept(*this);

    // try to find class of object? We don't have runtime type resolution here.
    // But if FieldAccess::Name() equals a known method name for current class, use that.
    std::string method_name = field_access->Name();
    // Try to find mangled in method_name_map_ by scanning for pattern "*::method_name"
    std::string found_mangled;
    for (const auto& kv : method_name_map_) {
      // kv.first is like "Class::method" or "Class::<ctor>"
      auto pos = kv.first.rfind("::");
      if (pos != std::string::npos) {
        std::string mname = kv.first.substr(pos + 2);
        if (mname == method_name) {
          found_mangled = kv.second;
          break;
        }
      }
    }
    if (!found_mangled.empty()) {
      EmitCommandWithString("CallVirtual", found_mangled);
    } else {
      EmitCommandWithString("CallVirtual", method_name);
    }
    return;
  }

  // generic fallback: evaluate callee then CallDynamic
  node.MutableCallee().Accept(*this);
  EmitCommand("CallDynamic");
}

void BytecodeVisitor::Visit(FieldAccess& node) {
  node.MutableObject().Accept(*this);
  // find field index if we have class info
  int index = 0;
  // try retrieving class name from object expression if it's IdentRef to 'this' or variable containing class instance
  // For now use placeholder 0 as before
  EmitCommandWithInt("GetField", index);
  // If field is primitive wrapper in our examples, emit Unwrap
  // we attempt to find field type if possible:
  // (not all cases available; left as heuristic)
  // If you have class_fields_ we could lookup by current_class_name_ or by object's static type
  // For now, don't automatically Unwrap here; let caller emit when needed.
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
  EmitCommandWithString("SafeCall", node.Method());
}

void BytecodeVisitor::Visit(Elvis& node) {
  // lhs ?: rhs
  node.MutableLhs().Accept(*this);
  node.MutableRhs().Accept(*this);
  EmitCommand("NullCoalesce");
}

void BytecodeVisitor::Visit(CastAs& node) {
  node.MutableExpression().Accept(*this);
  EmitCommand("CastAs");
}

void BytecodeVisitor::Visit(TypeTestIs& node) {
  node.MutableExpression().Accept(*this);
  std::string type_name = TypeToMangledName(node.Type());
  EmitCommandWithString("IsType", type_name);
}

void BytecodeVisitor::Visit(IdentRef& node) {
  // Prefer local; fallback to static/global
  if (local_variables_.find(node.Name()) != local_variables_.end()) {
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

void BytecodeVisitor::Visit(NullLit& node) {
  EmitCommand("PushNull");
}

void BytecodeVisitor::Visit(ThisExpr& node) {
  // TODO: MAKE bytecode visitor
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
                                              bool is_destructor) {
  std::ostringstream oss;
  if (is_constructor) {
    oss << "_" << class_name << "_constructor";
  } else if (is_destructor) {
    oss << "_" << class_name << "_destructor";
  } else {
    oss << "_" << class_name << "_" << method_name;
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
  oss << "_" << class_name << "_constructor";
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

std::string BytecodeVisitor::TypeToMangledName(const TypeReference& type) {
  std::ostringstream oss;
  const auto& qname = type.QualifiedName();
  for (size_t i = 0; i < qname.size(); ++i) {
    if (i > 0) {
      oss << "::";
    }
    oss << qname[i];
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
  next_local_index_ = 0;
}

} // namespace ovum::compiler::parser
