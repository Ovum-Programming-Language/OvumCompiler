// #include "BytecodeVisitor.hpp"

// #include <algorithm>
// #include <sstream>
// #include <string>
// #include <vector>

// #include "lib/parser/ast/nodes/base/Expr.hpp"
// #include "lib/parser/ast/nodes/base/Stmt.hpp"
// #include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
// #include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
// #include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
// #include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
// #include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"
// #include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
// #include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
// #include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
// #include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
// #include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"
// #include "lib/parser/ast/nodes/decls/Module.hpp"
// #include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"
// #include "lib/parser/ast/nodes/exprs/Binary.hpp"
// #include "lib/parser/ast/nodes/exprs/Unary.hpp"
// #include "lib/parser/ast/nodes/exprs/Assign.hpp"
// #include "lib/parser/ast/nodes/exprs/Call.hpp"
// #include "lib/parser/ast/nodes/exprs/FieldAccess.hpp"
// #include "lib/parser/ast/nodes/exprs/IndexAccess.hpp"
// #include "lib/parser/ast/nodes/exprs/NamespaceRef.hpp"
// #include "lib/parser/ast/nodes/exprs/SafeCall.hpp"
// #include "lib/parser/ast/nodes/exprs/Elvis.hpp"
// #include "lib/parser/ast/nodes/exprs/CastAs.hpp"
// #include "lib/parser/ast/nodes/exprs/TypeTestIs.hpp"
// #include "lib/parser/ast/nodes/exprs/IdentRef.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/IntLit.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/FloatLit.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/CharLit.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"
// #include "lib/parser/ast/nodes/exprs/literals/NullLit.hpp"
// #include "lib/parser/ast/nodes/stmts/Block.hpp"
// #include "lib/parser/ast/nodes/stmts/VarDeclStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/IfStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/ForStmt.hpp"
// #include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"
// #include "lib/parser/ast/nodes/exprs/tags/OpTags.hpp"
// #include "lib/parser/types/TypeReference.hpp"
// #include "lib/execution_tree/Block.hpp"
// #include "lib/execution_tree/Command.hpp"
// #include "lib/execution_tree/Function.hpp"
// #include "lib/execution_tree/command_factory.hpp"
// #include "lib/execution_tree/BytecodeCommands.hpp"
// #include "lib/execution_tree/IFunctionExecutable.hpp"
// #include "lib/execution_tree/ConditionalExecution.hpp"
// #include "lib/execution_tree/IfMultibranch.hpp"
// #include "lib/execution_tree/WhileExecution.hpp"
// #include "lib/runtime/FunctionId.hpp"

// namespace ovum::compiler::parser {

// BytecodeVisitor::BytecodeVisitor(ovum::vm::execution_tree::FunctionRepository& function_repository)
//     : function_repository_(function_repository) {
// }

// void BytecodeVisitor::Visit(Module& node) {
//   current_namespace_.clear();
//   for (auto& decl : node.MutableDecls()) {
//     decl->Accept(*this);
//   }
// }

// void BytecodeVisitor::Visit(FunctionDecl& node) {
//   ResetLocalVariables();
  
//   // Register parameters as local variables
//   size_t param_index = 0;
//   for (const auto& param : node.Params()) {
//     local_variables_[param.Name()] = param_index++;
//   }
  
//   // Create function body block
//   auto body_block = CreateBlock();
//   SetCurrentBlock(std::move(body_block));
  
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
  
//   // Create function
//   std::string func_id = GenerateFunctionId(node.Name(), node.Params());
//   auto func_body = std::move(current_block_);
//   auto func = std::make_unique<ovum::vm::execution_tree::Function>(
//       ovum::vm::runtime::FunctionId(func_id), node.Params().size(), std::move(func_body));
  
//   // Register function in repository (takes ownership)
//   function_repository_.Add(std::move(func));
// }

// void BytecodeVisitor::Visit(ClassDecl& node) {
//   std::string prev_class = current_class_name_;
//   current_class_name_ = node.Name();
  
//   for (auto& member : node.MutableMembers()) {
//     member->Accept(*this);
//   }
  
//   current_class_name_ = prev_class;
// }

// void BytecodeVisitor::Visit(InterfaceMethod& node) {
//   // Interface methods are just declarations, no implementation
// }

// void BytecodeVisitor::Visit(InterfaceDecl& node) {
//   // Interfaces are just declarations
// }

// void BytecodeVisitor::Visit(TypeAliasDecl& node) {
//   // Type aliases are compile-time only
// }

// void BytecodeVisitor::Visit(GlobalVarDecl& node) {
//   // Global variables are handled separately
//   if (node.Init() != nullptr) {
//     node.Init()->Accept(*this);
//     // Store in static variable
//     size_t static_index = GetStaticIndex(node.Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("SetStatic", static_cast<int64_t>(static_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(FieldDecl& node) {
//   // Fields are part of class structure, handled during class construction
// }

// void BytecodeVisitor::Visit(StaticFieldDecl& node) {
//   // Static fields are handled like global variables
//   if (node.Init() != nullptr) {
//     node.Init()->Accept(*this);
//     size_t static_index = GetStaticIndex(node.Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("SetStatic", static_cast<int64_t>(static_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(MethodDecl& node) {
//   ResetLocalVariables();
  
//   // Register 'this' as local 0 if not static
//   if (!node.IsStatic()) {
//     local_variables_["this"] = 0;
//   }
  
//   // Register parameters
//   size_t param_index = node.IsStatic() ? 0 : 1;
//   for (const auto& param : node.Params()) {
//     local_variables_[param.Name()] = param_index++;
//   }
  
//   // Create method body block
//   auto body_block = CreateBlock();
//   SetCurrentBlock(std::move(body_block));
  
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
  
//   // Create method function
//   std::string method_id = GenerateMethodId(current_class_name_, node.Name(), node.Params(), false, false);
//   auto method_body = std::move(current_block_);
//   auto method = std::make_unique<ovum::vm::execution_tree::Function>(
//       ovum::vm::runtime::FunctionId(method_id), node.Params().size() + (node.IsStatic() ? 0 : 1),
//       std::move(method_body));
  
//   // Register method in repository (takes ownership)
//   function_repository_.Add(std::move(method));
// }

// void BytecodeVisitor::Visit(CallDecl& node) {
//   ResetLocalVariables();
  
//   // Register 'this' as local 0
//   local_variables_["this"] = 0;
  
//   // Register parameters
//   size_t param_index = 1;
//   for (const auto& param : node.Params()) {
//     local_variables_[param.Name()] = param_index++;
//   }
  
//   // Create call body block
//   auto body_block = CreateBlock();
//   SetCurrentBlock(std::move(body_block));
  
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
  
//   // Create call function
//   std::string call_id = GenerateMethodId(current_class_name_, "call", node.Params(), false, false);
//   auto call_body = std::move(current_block_);
//   auto call_func = std::make_unique<ovum::vm::execution_tree::Function>(
//       ovum::vm::runtime::FunctionId(call_id), node.Params().size() + 1, std::move(call_body));
  
//   // Register call function in repository (takes ownership)
//   function_repository_.Add(std::move(call_func));
// }

// void BytecodeVisitor::Visit(DestructorDecl& node) {
//   ResetLocalVariables();
  
//   // Register 'this' as local 0
//   local_variables_["this"] = 0;
  
//   // Create destructor body block
//   auto body_block = CreateBlock();
//   SetCurrentBlock(std::move(body_block));
  
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
  
//   // Create destructor function
//   std::string destructor_id = GenerateDestructorId(current_class_name_);
//   auto destructor_body = std::move(current_block_);
//   auto destructor = std::make_unique<ovum::vm::execution_tree::Function>(
//       ovum::vm::runtime::FunctionId(destructor_id), 1, std::move(destructor_body));
  
//   // Register destructor in repository (takes ownership)
//   function_repository_.Add(std::move(destructor));
// }

// void BytecodeVisitor::Visit(Block& node) {
//   for (auto& stmt : node.MutableStatements()) {
//     stmt->Accept(*this);
//   }
// }

// void BytecodeVisitor::Visit(VarDeclStmt& node) {
//   if (node.Init() != nullptr) {
//     node.Init()->Accept(*this);
//     size_t local_index = GetLocalIndex(node.Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("SetLocal", static_cast<int64_t>(local_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(ExprStmt& node) {
//   if (node.Expr() != nullptr) {
//     node.Expr()->Accept(*this);
//     // Pop result if expression has side effects
//     auto pop_cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("Pop");
//     if (pop_cmd.has_value()) {
//       PushCommand(std::move(pop_cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(ReturnStmt& node) {
//   if (node.Value() != nullptr) {
//     node.Value()->Accept(*this);
//   }
//   auto return_cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("Return");
//   if (return_cmd.has_value()) {
//     PushCommand(std::move(return_cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(BreakStmt& node) {
//   // Break is handled by loop constructs
//   // TODO: Implement break handling
// }

// void BytecodeVisitor::Visit(ContinueStmt& node) {
//   // Continue is handled by loop constructs
//   // TODO: Implement continue handling
// }

// void BytecodeVisitor::Visit(IfStmt& node) {
//   auto if_multibranch = std::make_unique<ovum::vm::execution_tree::IfMultibranch>();
  
//   // Process all branches
//   for (const auto& branch : node.Branches()) {
//     // Create condition block
//     auto cond_block = CreateBlock();
//     auto prev_block = std::move(current_block_);
//     SetCurrentBlock(std::move(cond_block));
//     branch.Condition()->Accept(*this);
//     auto cond_block_final = std::move(current_block_);
    
//     // Create body block
//     auto body_block = CreateBlock();
//     SetCurrentBlock(std::move(body_block));
//     branch.Body()->Accept(*this);
//     auto body_block_final = std::move(current_block_);
    
//     // Create conditional execution
//     auto conditional = std::make_unique<ovum::vm::execution_tree::ConditionalExecution>(
//         std::move(cond_block_final), std::move(body_block_final));
//     if_multibranch->AddBranch(std::move(conditional));
    
//     SetCurrentBlock(std::move(prev_block));
//   }
  
//   // Process else block if present
//   if (node.ElseBlock() != nullptr) {
//     auto else_block = CreateBlock();
//     auto prev_block = std::move(current_block_);
//     SetCurrentBlock(std::move(else_block));
//     node.ElseBlock()->Accept(*this);
//     auto else_block_final = std::move(current_block_);
//     if_multibranch->SetElseBlock(std::move(else_block_final));
//     SetCurrentBlock(std::move(prev_block));
//   }
  
//   // Add if statement to current block
//   PushCommand(std::move(if_multibranch));
// }

// void BytecodeVisitor::Visit(WhileStmt& node) {
//   // Create condition block
//   auto cond_block = CreateBlock();
//   auto prev_block = std::move(current_block_);
//   SetCurrentBlock(std::move(cond_block));
//   if (node.Condition() != nullptr) {
//     node.Condition()->Accept(*this);
//   }
//   auto cond_block_final = std::move(current_block_);
  
//   // Create body block
//   auto body_block = CreateBlock();
//   SetCurrentBlock(std::move(body_block));
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
//   auto body_block_final = std::move(current_block_);
  
//   // Create while execution
//   auto while_exec = std::make_unique<ovum::vm::execution_tree::WhileExecution>(
//       std::move(cond_block_final), std::move(body_block_final));
  
//   SetCurrentBlock(std::move(prev_block));
//   PushCommand(std::move(while_exec));
// }

// void BytecodeVisitor::Visit(ForStmt& node) {
//   // TODO: Implement for loop
//   if (node.IterExpr() != nullptr) {
//     node.IterExpr()->Accept(*this);
//   }
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
// }

// void BytecodeVisitor::Visit(UnsafeBlock& node) {
//   if (node.Body() != nullptr) {
//     node.Body()->Accept(*this);
//   }
// }

// void BytecodeVisitor::Visit(Binary& node) {
//   // Push operands right-to-left
//   node.Rhs().Accept(*this);
//   node.Lhs().Accept(*this);
  
//   // Determine operation based on operator and type
//   std::string op_name = node.Op().Name();
  
//   // Map operator to bytecode command
//   // This is simplified - in real implementation, we'd need type information
//   if (op_name == "+") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntAdd");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "-") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntSubtract");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "*") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntMultiply");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "/") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntDivide");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "%") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntModulo");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "<") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntLessThan");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "<=") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntLessEqual");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == ">") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntGreaterThan");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == ">=") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntGreaterEqual");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "==") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntEqual");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "!=") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntNotEqual");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "&&") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("BoolAnd");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "||") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("BoolOr");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(Unary& node) {
//   node.Operand().Accept(*this);
  
//   std::string op_name = node.Op().Name();
  
//   if (op_name == "-") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("IntNegate");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else if (op_name == "!") {
//     auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("BoolNot");
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(Assign& node) {
//   // Evaluate value first
//   node.Value().Accept(*this);
  
//   // Then assign to target
//   // TODO: Handle different assignment types (reference vs copy)
//   if (auto* ident = dynamic_cast<IdentRef*>(&node.Target())) {
//     size_t local_index = GetLocalIndex(ident->Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("SetLocal", static_cast<int64_t>(local_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(Call& node) {
//   // Push arguments right-to-left
//   for (auto it = node.Args().rbegin(); it != node.Args().rend(); ++it) {
//     (*it)->Accept(*this);
//   }
  
//   // Call function
//   // TODO: Determine function name from callee
//   if (auto* ident = dynamic_cast<IdentRef*>(node.Callee())) {
//     std::string func_name = ident->Name();
//     auto cmd = ovum::vm::execution_tree::CreateStringCommandByName("Call", func_name);
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(FieldAccess& node) {
//   node.Object().Accept(*this);
//   // TODO: Implement field access
// }

// void BytecodeVisitor::Visit(IndexAccess& node) {
//   node.IndexExpr().Accept(*this);
//   node.Object().Accept(*this);
//   // TODO: Implement index access
// }

// void BytecodeVisitor::Visit(NamespaceRef& node) {
//   node.NamespaceExpr().Accept(*this);
//   // TODO: Implement namespace reference
// }

// void BytecodeVisitor::Visit(SafeCall& node) {
//   // Push arguments right-to-left
//   for (auto it = node.Args().rbegin(); it != node.Args().rend(); ++it) {
//     (*it)->Accept(*this);
//   }
  
//   node.Object().Accept(*this);
  
//   auto cmd = ovum::vm::execution_tree::CreateStringCommandByName("SafeCall", node.Method());
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(Elvis& node) {
//   node.Lhs().Accept(*this);
//   // TODO: Check for null and use rhs if null
//   node.Rhs().Accept(*this);
//   auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("NullCoalesce");
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(CastAs& node) {
//   node.Expr().Accept(*this);
//   // TODO: Implement type casting
// }

// void BytecodeVisitor::Visit(TypeTestIs& node) {
//   node.Expr().Accept(*this);
//   std::string type_name = TypeToMangledName(node.Type());
//   auto cmd = ovum::vm::execution_tree::CreateStringCommandByName("IsType", type_name);
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(IdentRef& node) {
//   // Check if it's a local variable first
//   if (local_variables_.find(node.Name()) != local_variables_.end()) {
//     size_t local_index = GetLocalIndex(node.Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("LoadLocal", static_cast<int64_t>(local_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   } else {
//     // Assume it's a global/static variable
//     size_t static_index = GetStaticIndex(node.Name());
//     auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("LoadStatic", static_cast<int64_t>(static_index));
//     if (cmd.has_value()) {
//       PushCommand(std::move(cmd.value()));
//     }
//   }
// }

// void BytecodeVisitor::Visit(IntLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("PushInt", node.Value());
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(FloatLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateFloatCommandByName("PushFloat", node.Value());
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(StringLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateStringCommandByName("PushString", node.Value());
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(CharLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateIntegerCommandByName("PushChar", static_cast<int64_t>(node.Value()));
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(BoolLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateBooleanCommandByName("PushBool", node.Value());
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// void BytecodeVisitor::Visit(NullLit& node) {
//   auto cmd = ovum::vm::execution_tree::CreateSimpleCommandByName("PushNull");
//   if (cmd.has_value()) {
//     PushCommand(std::move(cmd.value()));
//   }
// }

// std::vector<std::unique_ptr<ovum::vm::execution_tree::Function>> BytecodeVisitor::ReleaseFunctions() {
//   // Functions are owned by function_repository_, so return empty vector
//   // In a real implementation, you might want to extract functions from repository
//   return {};
// }

// void BytecodeVisitor::PushCommand(std::unique_ptr<ovum::vm::execution_tree::IExecutable> cmd) {
//   if (current_block_ != nullptr) {
//     current_block_->AddStatement(std::move(cmd));
//   }
// }

// std::string BytecodeVisitor::GenerateFunctionId(const std::string& name, const std::vector<Param>& params) {
//   std::ostringstream oss;
//   oss << name << "(";
//   for (size_t i = 0; i < params.size(); ++i) {
//     if (i > 0) {
//       oss << ",";
//     }
//     oss << TypeToMangledName(params[i].Type());
//   }
//   oss << ")";
//   return oss.str();
// }

// std::string BytecodeVisitor::GenerateMethodId(const std::string& class_name, const std::string& method_name,
//                                               const std::vector<Param>& params, bool is_constructor,
//                                               bool is_destructor) {
//   std::ostringstream oss;
//   oss << class_name << "::" << method_name << "(";
//   for (size_t i = 0; i < params.size(); ++i) {
//     if (i > 0) {
//       oss << ",";
//     }
//     oss << TypeToMangledName(params[i].Type());
//   }
//   oss << ")";
//   return oss.str();
// }

// std::string BytecodeVisitor::GenerateConstructorId(const std::string& class_name, const std::vector<Param>& params) {
//   return GenerateMethodId(class_name, class_name, params, true, false);
// }

// std::string BytecodeVisitor::GenerateDestructorId(const std::string& class_name) {
//   return class_name + "::~" + class_name + "()";
// }

// std::string BytecodeVisitor::TypeToMangledName(const TypeReference& type) {
//   std::ostringstream oss;
//   const auto& qname = type.QualifiedName();
//   for (size_t i = 0; i < qname.size(); ++i) {
//     if (i > 0) {
//       oss << ".";
//     }
//     oss << qname[i];
//   }
  
//   if (type.Arity() > 0) {
//     oss << "<";
//     for (size_t i = 0; i < type.Arity(); ++i) {
//       if (i > 0) {
//         oss << ",";
//       }
//       oss << TypeToMangledName(type.TypeArguments()[i]);
//     }
//     oss << ">";
//   }
  
//   if (type.IsNullable()) {
//     oss << "?";
//   }
  
//   return oss.str();
// }

// void BytecodeVisitor::VisitExpression(Expr* expr) {
//   if (expr != nullptr) {
//     expr->Accept(*this);
//   }
// }

// void BytecodeVisitor::VisitStatement(Stmt* stmt) {
//   if (stmt != nullptr) {
//     stmt->Accept(*this);
//   }
// }

// void BytecodeVisitor::VisitBlock(Block* block) {
//   if (block != nullptr) {
//     block->Accept(*this);
//   }
// }

// std::unique_ptr<ovum::vm::execution_tree::Block> BytecodeVisitor::CreateBlock() {
//   return std::make_unique<ovum::vm::execution_tree::Block>();
// }

// void BytecodeVisitor::SetCurrentBlock(std::unique_ptr<ovum::vm::execution_tree::Block> block) {
//   current_block_ = std::move(block);
// }

// size_t BytecodeVisitor::GetLocalIndex(const std::string& name) {
//   auto it = local_variables_.find(name);
//   if (it != local_variables_.end()) {
//     return it->second;
//   }
//   // If not found, allocate new index
//   size_t index = next_local_index_++;
//   local_variables_[name] = index;
//   return index;
// }

// size_t BytecodeVisitor::GetStaticIndex(const std::string& name) {
//   auto it = static_variables_.find(name);
//   if (it != static_variables_.end()) {
//     return it->second;
//   }
//   // If not found, allocate new index
//   size_t index = next_static_index_++;
//   static_variables_[name] = index;
//   return index;
// }

// void BytecodeVisitor::ResetLocalVariables() {
//   local_variables_.clear();
//   next_local_index_ = 0;
// }

// }  // namespace ovum::compiler::parser

