#include "BytecodeVisitor.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ranges>
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

constexpr size_t kPointerSizeBytes = 8;
constexpr size_t kIntFloatSizeBytes = 8;
constexpr size_t kByteCharBoolSizeBytes = 1;

size_t FieldSizeForType(const TypeReference& t) {
  if (t.QualifiedName().empty()) {
    return kPointerSizeBytes;
  }

  std::string_view type_name = t.SimpleName();

  if (type_name == "int" || type_name == "float") {
    return kIntFloatSizeBytes;
  }
  if (type_name == "byte" || type_name == "char" || type_name == "bool") {
    return kByteCharBoolSizeBytes;
  }

  return kPointerSizeBytes;
}

std::string EscapeStringForEmit(const std::string& s) {
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

const std::unordered_set<std::string> BytecodeVisitor::kBuiltinSystemCommands = {"Print",
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

const std::unordered_map<std::string, std::string> BytecodeVisitor::kBuiltinReturnPrimitives = {
    {"RandomFloatRange", "Float"},
    {"RandomFloat", "Float"},
    {"Random", "Int"},
    {"RandomRange", "Int"},
    {"UnixTime", "Int"},
    {"UnixTimeMs", "Int"},
    {"UnixTimeNs", "Int"},
    {"NanoTime", "Int"},
    {"GetProcessId", "Int"},
    {"GetMemoryUsage", "Int"},
    {"GetPeakMemoryUsage", "Int"},
    {"GetProcessorCount", "Int"},
    {"ParseDateTime", "Int"},
};

const std::unordered_set<std::string> BytecodeVisitor::kBuiltinTypeNames = {"Int",
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

const std::unordered_set<std::string> BytecodeVisitor::kPrimitiveTypeNames = {"int", "float", "byte", "char", "bool"};

const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> BytecodeVisitor::kBuiltinMethods = {
    {"String",
     {
         {"GetHash", "_String_GetHash_<C>"},
         {"ToString", "_String_ToString_<C>"},
         {"Length", "_String_Length_<C>"},
         {"Equals", "_String_Equals_<C>_Object"},
         {"IsLess", "_String_IsLess_<C>_Object"},
         {"Substring", "_String_Substring_<C>_int_int"},
         {"Compare", "_String_Compare_<C>_String"},
         {"ToUtf8Bytes", "_String_ToUtf8Bytes_<C>"},
     }},
    {"Int",
     {
         {"ToString", "_Int_ToString_<C>"},
         {"GetHash", "_Int_GetHash_<C>"},
         {"Equals", "_Int_Equals_<C>_Object"},
         {"IsLess", "_Int_IsLess_<C>_Object"},
     }},
    {"Float",
     {
         {"ToString", "_Float_ToString_<C>"},
         {"GetHash", "_Float_GetHash_<C>"},
         {"Equals", "_Float_Equals_<C>_Object"},
         {"IsLess", "_Float_IsLess_<C>_Object"},
     }},
    {"Byte",
     {
         {"ToString", "_Byte_ToString_<C>"},
         {"GetHash", "_Byte_GetHash_<C>"},
         {"Equals", "_Byte_Equals_<C>_Object"},
         {"IsLess", "_Byte_IsLess_<C>_Object"},
     }},
    {"Char",
     {
         {"ToString", "_Char_ToString_<C>"},
         {"GetHash", "_Char_GetHash_<C>"},
         {"Equals", "_Char_Equals_<C>_Object"},
         {"IsLess", "_Char_IsLess_<C>_Object"},
     }},
    {"Bool",
     {
         {"ToString", "_Bool_ToString_<C>"},
         {"GetHash", "_Bool_GetHash_<C>"},
         {"Equals", "_Bool_Equals_<C>_Object"},
         {"IsLess", "_Bool_IsLess_<C>_Object"},
     }},
    {"IntArray",
     {
         {"Length", "_IntArray_Length_<C>"},
         {"ToString", "_IntArray_ToString_<C>"},
         {"GetHash", "_IntArray_GetHash_<C>"},
         {"Equals", "_IntArray_Equals_<C>_Object"},
         {"IsLess", "_IntArray_IsLess_<C>_Object"},
         {"Clear", "_IntArray_Clear_<M>"},
         {"ShrinkToFit", "_IntArray_ShrinkToFit_<M>"},
         {"Reserve", "_IntArray_Reserve_<M>_int"},
         {"Capacity", "_IntArray_Capacity_<C>"},
         {"Add", "_IntArray_Add_<M>_int"},
         {"RemoveAt", "_IntArray_RemoveAt_<M>_int"},
         {"InsertAt", "_IntArray_InsertAt_<M>_int_int"},
         {"SetAt", "_IntArray_SetAt_<M>_int_int"},
         {"GetAt", "_IntArray_GetAt_<C>_int"},
     }},
    {"FloatArray",
     {
         {"Length", "_FloatArray_Length_<C>"},
         {"ToString", "_FloatArray_ToString_<C>"},
         {"GetHash", "_FloatArray_GetHash_<C>"},
         {"Equals", "_FloatArray_Equals_<C>_Object"},
         {"IsLess", "_FloatArray_IsLess_<C>_Object"},
         {"Clear", "_FloatArray_Clear_<M>"},
         {"ShrinkToFit", "_FloatArray_ShrinkToFit_<M>"},
         {"Reserve", "_FloatArray_Reserve_<M>_int"},
         {"Capacity", "_FloatArray_Capacity_<C>"},
         {"Add", "_FloatArray_Add_<M>_float"},
         {"RemoveAt", "_FloatArray_RemoveAt_<M>_int"},
         {"InsertAt", "_FloatArray_InsertAt_<M>_int_float"},
         {"SetAt", "_FloatArray_SetAt_<M>_int_float"},
         {"GetAt", "_FloatArray_GetAt_<C>_int"},
     }},
    {"ByteArray",
     {
         {"Length", "_ByteArray_Length_<C>"},
         {"ToString", "_ByteArray_ToString_<C>"},
         {"GetHash", "_ByteArray_GetHash_<C>"},
         {"Equals", "_ByteArray_Equals_<C>_Object"},
         {"IsLess", "_ByteArray_IsLess_<C>_Object"},
         {"Clear", "_ByteArray_Clear_<M>"},
         {"ShrinkToFit", "_ByteArray_ShrinkToFit_<M>"},
         {"Reserve", "_ByteArray_Reserve_<M>_int"},
         {"Capacity", "_ByteArray_Capacity_<C>"},
         {"Add", "_ByteArray_Add_<M>_byte"},
         {"RemoveAt", "_ByteArray_RemoveAt_<M>_int"},
         {"InsertAt", "_ByteArray_InsertAt_<M>_int_byte"},
         {"SetAt", "_ByteArray_SetAt_<M>_int_byte"},
         {"GetAt", "_ByteArray_GetAt_<C>_int"},
     }},
    {"BoolArray",
     {
         {"Length", "_BoolArray_Length_<C>"},
         {"ToString", "_BoolArray_ToString_<C>"},
         {"GetHash", "_BoolArray_GetHash_<C>"},
         {"Equals", "_BoolArray_Equals_<C>_Object"},
         {"IsLess", "_BoolArray_IsLess_<C>_Object"},
         {"Clear", "_BoolArray_Clear_<M>"},
         {"ShrinkToFit", "_BoolArray_ShrinkToFit_<M>"},
         {"Reserve", "_BoolArray_Reserve_<M>_int"},
         {"Capacity", "_BoolArray_Capacity_<C>"},
         {"Add", "_BoolArray_Add_<M>_bool"},
         {"RemoveAt", "_BoolArray_RemoveAt_<M>_int"},
         {"InsertAt", "_BoolArray_InsertAt_<M>_int_bool"},
         {"SetAt", "_BoolArray_SetAt_<M>_int_bool"},
         {"GetAt", "_BoolArray_GetAt_<C>_int"},
     }},
    {"CharArray",
     {
         {"Length", "_CharArray_Length_<C>"},
         {"ToString", "_CharArray_ToString_<C>"},
         {"GetHash", "_CharArray_GetHash_<C>"},
         {"Equals", "_CharArray_Equals_<C>_Object"},
         {"IsLess", "_CharArray_IsLess_<C>_Object"},
         {"Clear", "_CharArray_Clear_<M>"},
         {"ShrinkToFit", "_CharArray_ShrinkToFit_<M>"},
         {"Reserve", "_CharArray_Reserve_<M>_int"},
         {"Capacity", "_CharArray_Capacity_<C>"},
         {"Add", "_CharArray_Add_<M>_char"},
         {"RemoveAt", "_CharArray_RemoveAt_<M>_int"},
         {"InsertAt", "_CharArray_InsertAt_<M>_int_char"},
         {"SetAt", "_CharArray_SetAt_<M>_int_char"},
         {"GetAt", "_CharArray_GetAt_<C>_int"},
     }},
    {"StringArray",
     {
         {"Length", "_StringArray_Length_<C>"},
         {"ToString", "_StringArray_ToString_<C>"},
         {"GetHash", "_StringArray_GetHash_<C>"},
         {"Equals", "_StringArray_Equals_<C>_Object"},
         {"IsLess", "_StringArray_IsLess_<C>_Object"},
         {"Clear", "_StringArray_Clear_<M>"},
         {"ShrinkToFit", "_StringArray_ShrinkToFit_<M>"},
         {"Reserve", "_StringArray_Reserve_<M>_int"},
         {"Capacity", "_StringArray_Capacity_<C>"},
         {"Add", "_StringArray_Add_<M>_String"},
         {"RemoveAt", "_StringArray_RemoveAt_<M>_int"},
         {"InsertAt", "_StringArray_InsertAt_<M>_int_String"},
         {"SetAt", "_StringArray_SetAt_<M>_int_String"},
         {"GetAt", "_StringArray_GetAt_<C>_int"},
     }},
    {"ObjectArray",
     {
         {"Length", "_ObjectArray_Length_<C>"},
         {"ToString", "_ObjectArray_ToString_<C>"},
         {"GetHash", "_ObjectArray_GetHash_<C>"},
         {"Equals", "_ObjectArray_Equals_<C>_Object"},
         {"IsLess", "_ObjectArray_IsLess_<C>_Object"},
         {"Clear", "_ObjectArray_Clear_<M>"},
         {"ShrinkToFit", "_ObjectArray_ShrinkToFit_<M>"},
         {"Reserve", "_ObjectArray_Reserve_<M>_int"},
         {"Capacity", "_ObjectArray_Capacity_<C>"},
         {"Add", "_ObjectArray_Add_<M>_Object"},
         {"RemoveAt", "_ObjectArray_RemoveAt_<M>_int"},
         {"InsertAt", "_ObjectArray_InsertAt_<M>_int_Object"},
         {"SetAt", "_ObjectArray_SetAt_<M>_int_Object"},
         {"GetAt", "_ObjectArray_GetAt_<C>_int"},
     }},
    {"File",
     {
         {"Open", "_File_Open_<M>_String_String"},
         {"Close", "_File_Close_<M>"},
         {"IsOpen", "_File_IsOpen_<C>"},
         {"Read", "_File_Read_<M>_Int"},
         {"Write", "_File_Write_<M>_ByteArray"},
         {"ReadLine", "_File_ReadLine_<M>"},
         {"WriteLine", "_File_WriteLine_<M>_String"},
         {"Seek", "_File_Seek_<M>_Int"},
         {"Tell", "_File_Tell_<C>"},
         {"Eof", "_File_Eof_<C>"},
     }},
};

BytecodeVisitor::BytecodeVisitor(std::ostream& output) : output_(output), pending_init_static_({}) {
}

void BytecodeVisitor::EmitIndent() const {
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
  output_ << command << " ";

  if (value == std::floor(value) && std::isfinite(value)) {
    output_ << value << ".0";
  } else {
    const auto default_precision{output_.precision()};
    output_ << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10 - 2) << value;
    output_ << std::setprecision(default_precision);
    output_.unsetf(std::ios::fixed);
  }
  output_ << "\n";
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

void BytecodeVisitor::EmitPushByte(uint8_t value) {
  EmitCommandWithInt("PushByte", value);
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
  function_name_map_.clear();
  function_return_types_.clear();
  function_overloads_.clear();
  method_name_map_.clear();
  method_vtable_map_.clear();
  method_return_types_.clear();
  class_fields_.clear();
  constructor_params_.clear();
  type_aliases_.clear();
  pending_init_static_.clear();
  pending_init_static_names_.clear();
  pending_init_static_types_.clear();

  for (auto& decl : node.MutableDecls()) {
    if (auto* f = dynamic_cast<FunctionDecl*>(decl.get())) {
      std::string mangled = GenerateFunctionId(f->Name(), f->Params());
      function_name_map_[f->Name()] = mangled; // Keep for backward compatibility

      FunctionOverload overload;
      overload.mangled_name = mangled;
      for (const auto& param : f->Params()) {
        overload.param_types.push_back(param.GetType());
      }
      if (f->ReturnType() != nullptr) {
        overload.return_type = TypeToMangledName(*f->ReturnType());
        function_return_types_[f->Name()] = overload.return_type; // Keep last one for backward compatibility
      } else {
        overload.return_type = "void";
        function_return_types_[f->Name()] = "void";
      }
      function_overloads_[f->Name()].push_back(std::move(overload));
    }

    if (auto* c = dynamic_cast<ClassDecl*>(decl.get())) {
      std::string class_name = c->Name();

      std::vector<std::pair<std::string, TypeReference>> fields;
      for (auto& m : c->MutableMembers()) {
        if (const auto* fd = dynamic_cast<FieldDecl*>(m.get())) {
          fields.emplace_back(fd->Name(), fd->Type());
        }
        if (auto* sd = dynamic_cast<StaticFieldDecl*>(m.get())) {
          if (sd->MutableInit() != nullptr) {
            pending_init_static_.push_back(sd->MutableInit());
            pending_init_static_names_.push_back(sd->Name());
            pending_init_static_types_.push_back(sd->Type());
          }
        }
        if (const auto* md = dynamic_cast<MethodDecl*>(m.get())) {
          if (md->Name() == class_name) {
            const std::string ctor = GenerateConstructorId(class_name, md->Params());
            method_name_map_[class_name + "::<ctor>"] = ctor;

            std::vector<TypeReference> param_types;
            for (const auto& param : md->Params()) {
              param_types.push_back(param.GetType());
            }
            constructor_params_[class_name + "::<ctor>"] = param_types;
          } else {
            bool is_mutable = false;
            std::string mid = GenerateMethodId(class_name, md->Name(), md->Params(), false, false, is_mutable);
            std::string vtable_name = GenerateMethodVTableName(md->Name(), md->Params(), is_mutable);
            method_name_map_[class_name + "::" + md->Name()] = mid;
            method_vtable_map_[class_name + "::" + md->Name()] = vtable_name;

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

          std::vector<TypeReference> param_types;
          for (const auto& param : cd->Params()) {
            param_types.push_back(param.GetType());
          }
          constructor_params_[class_name + "::<ctor>"] = param_types;
        }
      }
      class_fields_[class_name] = fields;
    }

    if (auto* gv = dynamic_cast<GlobalVarDecl*>(decl.get())) {
      if (gv->MutableInit() != nullptr) {
        pending_init_static_.push_back(gv->MutableInit());
        pending_init_static_names_.push_back(gv->Name());
        pending_init_static_types_.push_back(gv->Type());
      }
    }

    if (auto* ta = dynamic_cast<TypeAliasDecl*>(decl.get())) {
      type_aliases_[ta->Name()] = ta->AliasedType();
    }
  }

  EmitIndent();
  output_ << "init-static {\n";
  indent_level_++;
  if (!pending_init_static_.empty()) {
    for (size_t i = 0; i < pending_init_static_.size(); ++i) {
      pending_init_static_[i]->Accept(*this);

      // Add CallConstructor if type is a wrapper type (Float, Int, etc.)
      if (i < pending_init_static_types_.size()) {
        std::string type_name = TypeToMangledName(pending_init_static_types_[i]);
        if (IsPrimitiveWrapper(type_name)) {
          std::string primitive_type = GetPrimitiveTypeForWrapper(type_name);
          EmitWrapConstructorCall(type_name, primitive_type);
        }
        variable_types_[pending_init_static_names_[i]] = type_name;
      }

      EmitCommandWithInt("SetStatic", static_cast<int64_t>(GetStaticIndex(pending_init_static_names_[i])));
    }
  }
  indent_level_--;
  EmitIndent();
  output_ << "}\n\n";

  for (auto& decl : node.MutableDecls()) {
    decl->Accept(*this);
  }
}

void BytecodeVisitor::Visit(FunctionDecl& node) {
  ResetLocalVariables();

  std::string prev_function = current_function_name_;
  current_function_name_ = node.Name();

  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());

    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  std::string mangled = GenerateFunctionId(node.Name(), node.Params());
  function_name_map_[node.Name()] = mangled; // Keep for backward compatibility

  FunctionOverload overload;
  overload.mangled_name = mangled;
  for (const auto& param : node.Params()) {
    overload.param_types.push_back(param.GetType());
  }
  if (node.ReturnType() != nullptr) {
    overload.return_type = TypeToMangledName(*node.ReturnType());
    function_return_types_[node.Name()] = overload.return_type; // Keep last one for backward compatibility
  } else {
    overload.return_type = "void";
    function_return_types_[node.Name()] = "void";
  }
  function_overloads_[node.Name()].push_back(std::move(overload));

  if (node.IsPure()) {
    output_ << "pure";

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

  current_function_name_ = prev_function;
}

void BytecodeVisitor::Visit(MethodDecl& node) {
  if (node.Name() == current_class_name_) {
    ResetLocalVariables();
    GetLocalIndex("this");
    for (auto& param : node.MutableParams()) {
      GetLocalIndex(param.GetName());

      std::string type_name = TypeToMangledName(param.GetType());
      variable_types_[param.GetName()] = type_name;
    }

    std::string ctor_id = GenerateConstructorId(current_class_name_, node.Params());
    method_name_map_[current_class_name_ + "::<ctor>"] = ctor_id;

    std::vector<TypeReference> param_types;
    for (const auto& param : node.Params()) {
      param_types.push_back(param.GetType());
    }
    constructor_params_[current_class_name_ + "::<ctor>"] = param_types;

    output_ << "function:" << (node.Params().size() + 1) << " " << ctor_id << " ";
    EmitBlockStart();
    if (node.MutableBody() != nullptr) {
      bool has_return = false;
      if (const auto& stmts = node.MutableBody()->GetStatements(); !stmts.empty()) {
        if (dynamic_cast<ReturnStmt*>(stmts.back().get()) != nullptr) {
          has_return = true;
        }
      }

      node.MutableBody()->Accept(*this);

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

  ResetLocalVariables();

  GetLocalIndex("this");
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());

    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  bool is_mutable = false;

  std::string method_id = GenerateMethodId(current_class_name_, node.Name(), node.Params(), false, false, is_mutable);

  method_name_map_[current_class_name_ + "::" + node.Name()] = method_id;

  output_ << "function:" << (node.Params().size() + 1) << " " << method_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(CallDecl& node) {
  ResetLocalVariables();

  GetLocalIndex("this");
  for (auto& param : node.MutableParams()) {
    GetLocalIndex(param.GetName());

    std::string type_name = TypeToMangledName(param.GetType());
    variable_types_[param.GetName()] = type_name;
  }

  std::string call_id = GenerateConstructorId(current_class_name_, node.Params());

  method_name_map_[current_class_name_ + "::<ctor>"] = call_id;

  std::vector<TypeReference> param_types;
  for (const auto& param : node.Params()) {
    param_types.push_back(param.GetType());
  }
  constructor_params_[current_class_name_ + "::<ctor>"] = param_types;

  output_ << "function:" << (node.Params().size() + 1) << " " << call_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);

    // Check if body ends with return statement
    bool has_return = false;
    if (const auto& stmts = node.MutableBody()->GetStatements(); !stmts.empty()) {
      if (dynamic_cast<ReturnStmt*>(stmts.back().get()) != nullptr) {
        has_return = true;
      }
    }

    // Only add implicit return if body doesn't already have one
    if (!has_return) {
      if (node.ReturnType() != nullptr) {
        std::string return_type_name = TypeToMangledName(*node.ReturnType());
        if (return_type_name == "void") {
          EmitCommand("Return");
        } else {
          EmitCommandWithInt("LoadLocal", 0);
          EmitCommand("Return");
        }
      } else {
        EmitCommand("Return");
      }
    }
  } else {
    if (node.ReturnType() != nullptr) {
      std::string return_type_name = TypeToMangledName(*node.ReturnType());
      if (return_type_name == "void") {
        EmitCommand("Return");
      } else {
        EmitCommandWithInt("LoadLocal", 0);
        EmitCommand("Return");
      }
    } else {
      EmitCommand("Return");
    }
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
  bool is_mutable = false;
  std::string id = GenerateMethodId(
      /* class_name */ "",
      node.Name(),
      node.MutableParams(),
      /* is_constructor */ false,
      /* is_destructor */ false,
      is_mutable);

  method_name_map_[node.Name()] = id;
}

void BytecodeVisitor::Visit(TypeAliasDecl& node) {
  type_aliases_[node.Name()] = node.AliasedType();
}

void BytecodeVisitor::Visit(GlobalVarDecl& node) {
  (void) GetStaticIndex(node.Name());
  std::string type_name = TypeToMangledName(node.Type());
  variable_types_[node.Name()] = type_name;
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

  GetLocalIndex("this");

  std::string destructor_id = GenerateDestructorId(current_class_name_);
  method_name_map_[current_class_name_ + "::<dtor>"] = destructor_id;

  output_ << "function:1 " << destructor_id << " ";
  EmitBlockStart();
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
  EmitBlockEnd();
  output_ << "\n";
}

void BytecodeVisitor::Visit(ClassDecl& node) {
  std::string prev_class = current_class_name_;
  current_class_name_ = node.Name();

  std::vector<std::pair<std::string, TypeReference>> fields;
  std::vector<std::pair<std::string, std::string>> method_map;

  for (auto& member : node.MutableMembers()) {
    if (auto* fd = dynamic_cast<FieldDecl*>(member.get())) {
      fields.emplace_back(fd->Name(), fd->Type());
    }
    if (auto* ifm = dynamic_cast<InterfaceMethod*>(member.get())) {
      (void) ifm;
    }
    if (auto* md = dynamic_cast<MethodDecl*>(member.get())) {
      if (md->Name() == node.Name()) {
        std::string ctor_id = GenerateConstructorId(node.Name(), md->Params());
        method_name_map_[node.Name() + "::<ctor>"] = ctor_id;

        std::vector<TypeReference> param_types;
        for (const auto& param : md->Params()) {
          param_types.push_back(param.GetType());
        }
        constructor_params_[node.Name() + "::<ctor>"] = param_types;

      } else {
        bool is_mutable = false;
        std::string mid = GenerateMethodId(node.Name(), md->Name(), md->Params(), false, false, is_mutable);
        std::string vtable_name = GenerateMethodVTableName(md->Name(), md->Params(), is_mutable);

        method_map.emplace_back(md->Name(), mid);
        method_name_map_[node.Name() + "::" + md->Name()] = mid;
        method_vtable_map_[node.Name() + "::" + md->Name()] = vtable_name;
      }
    }
    if (auto* cd = dynamic_cast<CallDecl*>(member.get())) {
      std::string ce = GenerateConstructorId(node.Name(), cd->Params());

      method_name_map_[node.Name() + "::<ctor>"] = ce;

      std::vector<TypeReference> param_types;
      for (const auto& param : cd->Params()) {
        param_types.push_back(param.GetType());
      }
      constructor_params_[node.Name() + "::<ctor>"] = param_types;
    }
    if (auto* sfd = dynamic_cast<StaticFieldDecl*>(member.get())) {
      if (sfd->MutableInit() != nullptr) {
        pending_init_static_.push_back(sfd->MutableInit());
        pending_init_static_names_.push_back(sfd->Name());
      }
    }
  }

  size_t header = 4 + 4;
  size_t fields_size = 0;
  for (auto& val : fields | std::views::values) {
    fields_size += FieldSizeForType(val);
  }
  size_t total_size = header + fields_size;

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
  }

  if (!method_map.empty()) {
    EmitIndent();
    output_ << "methods {\n";
    indent_level_++;
    for (auto& [fst, snd] : method_map) {
      EmitIndent();

      std::string vtable_name;
      for (auto& member : node.MutableMembers()) {
        if (auto* md = dynamic_cast<MethodDecl*>(member.get())) {
          if (md->Name() == fst) {
            bool is_mutable = false;
            vtable_name = GenerateMethodVTableName(fst, md->Params(), is_mutable);
            break;
          }
        }
      }
      output_ << vtable_name << ": " << snd << "\n";
    }
    indent_level_--;
    EmitIndent();
    output_ << "}\n";
  }

  EmitIndent();
  output_ << "vartable {\n";
  indent_level_++;

  size_t offset = header;
  for (auto& [fst, snd] : fields) {
    EmitIndent();

    std::string tname = TypeToMangledName(snd);
    std::string vartable_type;
    if (tname == "int" || tname == "float" || tname == "byte" || tname == "char" || tname == "bool") {
      vartable_type = tname;
    } else {
      vartable_type = "Object";
    }
    output_ << fst << ": " << vartable_type << "@" << offset << "\n";
    offset += FieldSizeForType(snd);
  }
  indent_level_--;
  EmitIndent();
  output_ << "}\n";

  indent_level_--;
  EmitIndent();
  output_ << "}\n\n";

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

    std::string expected_type_name = TypeToMangledName(node.Type());
    std::string value_type_name = GetTypeNameForExpr(node.MutableInit());
    EmitTypeConversionIfNeeded(expected_type_name, value_type_name);

    EmitCommandWithInt("SetLocal", static_cast<int64_t>(GetLocalIndex(node.Name())));
  }

  std::string type_name = TypeToMangledName(node.Type());
  variable_types_[node.Name()] = type_name;
}

void BytecodeVisitor::Visit(ExprStmt& node) {
  if (node.MutableExpression() != nullptr) {
    bool is_system_command = false;
    if (auto* call = dynamic_cast<Call*>(node.MutableExpression())) {
      if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&call->MutableCallee())) {
        is_system_command = IsBuiltinSystemCommand(ns_ref->Name());
      }
    }

    node.MutableExpression()->Accept(*this);

    bool should_pop = true;

    if (is_system_command || dynamic_cast<Assign*>(node.MutableExpression())) {
      should_pop = false;
    } else if (auto* call = dynamic_cast<Call*>(node.MutableExpression())) {
      if (const auto* ident = dynamic_cast<IdentRef*>(&call->MutableCallee())) {
        const std::string func_name = ident->Name();
        if (const auto return_type_it = function_return_types_.find(func_name);
            return_type_it != function_return_types_.end()) {
          if (return_type_it->second == "void" || return_type_it->second == "Void") {
            should_pop = false;
          }
        }
      } else if (auto* field_access = dynamic_cast<FieldAccess*>(&call->MutableCallee())) {
        const std::string method_name = field_access->Name();

        // Use GetTypeNameForExpr to determine object type, which handles chained calls
        std::string object_type = GetTypeNameForExpr(&field_access->MutableObject());

        // If object_type is "unknown" (from a Call expression), try to resolve it for chained calls
        if (object_type == "unknown" || object_type.empty()) {
          if (auto* nested_call = dynamic_cast<Call*>(&field_access->MutableObject())) {
            if (auto* nested_field_access = dynamic_cast<FieldAccess*>(&nested_call->MutableCallee())) {
              std::string nested_method_name = nested_field_access->Name();
              std::string nested_object_type = GetTypeNameForExpr(&nested_field_access->MutableObject());
              
              if (!nested_object_type.empty() && !kBuiltinTypeNames.contains(nested_object_type)) {
                std::string nested_method_key = nested_object_type + "::" + nested_method_name;
                if (const auto nested_it = method_return_types_.find(nested_method_key); nested_it != method_return_types_.end()) {
                  object_type = nested_it->second;
                }
              }
            }
          }
        }

        std::string method_key;
        if (!object_type.empty() && !kBuiltinTypeNames.contains(object_type)) {
          method_key = object_type + "::" + method_name;
        } else if (!current_class_name_.empty()) {
          method_key = current_class_name_ + "::" + method_name;
        }

        if (!method_key.empty()) {
          if (const auto return_type_it = method_return_types_.find(method_key);
              return_type_it != method_return_types_.end()) {
            if (return_type_it->second == "void" || return_type_it->second == "Void") {
              should_pop = false;
            }
          }
        }
      }
    }
    // If should_pop is still true at this point, it means the expression left a value on the stack

    if (should_pop) {
      EmitCommand("Pop");
    }
  }
}

void BytecodeVisitor::Visit(ReturnStmt& node) {
  if (node.MutableValue() != nullptr) {
    node.MutableValue()->Accept(*this);

    if (!current_function_name_.empty()) {
      if (const auto it = function_return_types_.find(current_function_name_); it != function_return_types_.end()) {
        const std::string return_type = it->second;
        const std::string result_type_name = GetTypeNameForExpr(node.MutableValue());
        EmitTypeConversionIfNeeded(return_type, result_type_name);
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
    if (node.MutableElseBlock()) {
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
  size_t collection_index = 0;
  std::string collection_var_name = node.IteratorName() + "_collection";
  std::string collection_type;

  if (node.MutableIteratorExpr() != nullptr) {
    if (const auto* ident = dynamic_cast<IdentRef*>(node.MutableIteratorExpr())) {
      const auto var_it = variable_types_.find(ident->Name());
      if (const auto local_it = local_variables_.find(ident->Name());
          var_it != variable_types_.end() && local_it != local_variables_.end()) {
        collection_index = local_it->second;
        collection_type = var_it->second;
      } else {
        node.MutableIteratorExpr()->Accept(*this);
        collection_index = GetLocalIndex(collection_var_name);
        EmitCommandWithInt("SetLocal", static_cast<int64_t>(collection_index));

        collection_type = "ObjectArray";
      }
    } else {
      node.MutableIteratorExpr()->Accept(*this);
      collection_index = GetLocalIndex(collection_var_name);
      EmitCommandWithInt("SetLocal", static_cast<int64_t>(collection_index));

      collection_type = "ObjectArray";
    }
  }

  EmitCommandWithInt("PushInt", 0);
  size_t counter_index = GetLocalIndex(node.IteratorName() + "_i");
  variable_types_[node.IteratorName() + "_i"] = "int";
  EmitCommandWithInt("SetLocal", static_cast<int64_t>(counter_index));

  EmitIndent();
  output_ << "while ";
  EmitBlockStartWithoutSpaces();

  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(collection_index));

  std::string length_method = GenerateArrayLengthMethodName(collection_type);
  EmitCommandWithStringWithoutBraces("Call", length_method);

  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(counter_index));

  EmitCommand("IntLessThan");

  EmitBlockEndWithoutEscape();
  output_ << " then ";
  EmitBlockStartWithoutSpaces();

  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(counter_index));
  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(collection_index));

  if (collection_type.empty()) {
    collection_type = "ObjectArray";
  }

  std::string method_name = GenerateArrayGetAtMethodName(collection_type);
  EmitCommandWithStringWithoutBraces("Call", method_name);

  size_t item_index = GetLocalIndex(node.IteratorName());
  variable_types_[node.IteratorName()] = GetElementTypeForArray(collection_type);
  EmitCommandWithInt("SetLocal", static_cast<int64_t>(item_index));

  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }

  EmitCommandWithInt("PushInt", 1);
  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(counter_index));
  EmitCommand("IntAdd");
  EmitCommandWithInt("SetLocal", static_cast<int64_t>(counter_index));

  EmitBlockEnd();
}

void BytecodeVisitor::Visit(UnsafeBlock& node) {
  if (node.MutableBody() != nullptr) {
    node.MutableBody()->Accept(*this);
  }
}

void BytecodeVisitor::Visit(Binary& node) {
  const auto& op = node.Op();

  bool is_null_comparison =
      dynamic_cast<NullLit*>(&node.MutableLhs()) != nullptr || dynamic_cast<NullLit*>(&node.MutableRhs()) != nullptr;

  if (is_null_comparison && (&op == &optags::Eq() || &op == &optags::Ne())) {
    if (dynamic_cast<NullLit*>(&node.MutableLhs()) != nullptr) {
      node.MutableRhs().Accept(*this);
    } else {
      node.MutableLhs().Accept(*this);
    }

    EmitCommand("IsNull");
    if (&op == &optags::Ne()) {
      EmitCommand("BoolNot");
    }
    return;
  }

  std::string lhs_type_name = GetTypeNameForExpr(&node.MutableLhs());
  std::string rhs_type_name = GetTypeNameForExpr(&node.MutableRhs());

  OperandType lhs_type = DetermineOperandType(&node.MutableLhs());
  OperandType rhs_type = DetermineOperandType(&node.MutableRhs());

  if (IsPrimitiveWrapper(lhs_type_name)) {
    if (lhs_type_name == "Int") {
      lhs_type = OperandType::kInt;
    } else if (lhs_type_name == "Float") {
      lhs_type = OperandType::kFloat;
    } else if (lhs_type_name == "Byte") {
      lhs_type = OperandType::kByte;
    } else if (lhs_type_name == "Char") {
      lhs_type = OperandType::kChar;
    } else if (lhs_type_name == "Bool") {
      lhs_type = OperandType::kBool;
    }
  }
  if (IsPrimitiveWrapper(rhs_type_name)) {
    if (rhs_type_name == "Int") {
      rhs_type = OperandType::kInt;
    } else if (rhs_type_name == "Float") {
      rhs_type = OperandType::kFloat;
    } else if (rhs_type_name == "Byte") {
      rhs_type = OperandType::kByte;
    } else if (rhs_type_name == "Char") {
      rhs_type = OperandType::kChar;
    } else if (rhs_type_name == "Bool") {
      rhs_type = OperandType::kBool;
    }
  }

  bool is_comparison = &op == &optags::Lt() || &op == &optags::Le() || &op == &optags::Gt() || &op == &optags::Ge() ||
                       &op == &optags::Eq() || &op == &optags::Ne();
  bool is_bitwise = &op == &optags::BitwiseAnd() || &op == &optags::BitwiseOr() || &op == &optags::Xor() ||
                    &op == &optags::LeftShift() || &op == &optags::RightShift();

  OperandType dominant_type = lhs_type;
  if (rhs_type == OperandType::kFloat || lhs_type == OperandType::kFloat) {
    dominant_type = OperandType::kFloat;
  } else if (((is_comparison || is_bitwise) && (rhs_type == OperandType::kByte || lhs_type == OperandType::kByte)) ||
             rhs_type == OperandType::kByte || lhs_type == OperandType::kByte) {
    dominant_type = OperandType::kByte;
  } else if (rhs_type == OperandType::kInt || lhs_type == OperandType::kInt) {
    dominant_type = OperandType::kInt;
  } else if (rhs_type == OperandType::kString || lhs_type == OperandType::kString) {
    dominant_type = OperandType::kString;
  } else if (rhs_type == OperandType::kBool || lhs_type == OperandType::kBool) {
    dominant_type = OperandType::kBool;
  }

  if (&op == &optags::Add() && dominant_type == OperandType::kString) {
    node.MutableRhs().Accept(*this);
    EmitUnwrapIfNeeded(rhs_type_name);
    node.MutableLhs().Accept(*this);
    EmitUnwrapIfNeeded(lhs_type_name);
    EmitCommand("StringConcat");
    return;
  }

  node.MutableRhs().Accept(*this);
  EmitUnwrapIfNeeded(rhs_type_name);
  node.MutableLhs().Accept(*this);
  EmitUnwrapIfNeeded(lhs_type_name);
  EmitBinaryOperatorCommand(op, dominant_type);
}

void BytecodeVisitor::Visit(Unary& node) {
  const auto& op = node.Op();

  if (&op == &optags::Unwrap()) {
    node.MutableOperand().Accept(*this);

    EmitCommand("Unwrap");
    return;
  }

  std::string operand_type_name = GetTypeNameForExpr(&node.MutableOperand());

  node.MutableOperand().Accept(*this);

  if (IsPrimitiveWrapper(operand_type_name)) {
    EmitCommand("Unwrap");
  }

  OperandType operand_type = DetermineOperandType(&node.MutableOperand());

  if (&op == &optags::Neg()) {
    if (operand_type == OperandType::kFloat) {
      EmitCommand("FloatNegate");
    } else if (operand_type == OperandType::kByte) {
      EmitCommand("ByteNegate");
    } else {
      EmitCommand("IntNegate");
    }

    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name);
    }
  } else if (&op == &optags::Plus()) {
    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name);
    }
  } else if (&op == &optags::Not()) {
    EmitCommand("BoolNot");
  } else if (&op == &optags::BitwiseNot()) {
    if (operand_type == OperandType::kByte) {
      EmitCommand("ByteNot");
    } else {
      EmitCommand("IntNot");
    }

    if (IsPrimitiveWrapper(operand_type_name)) {
      EmitWrapIfNeeded(operand_type_name);
    }
  } else {
    EmitCommand("UnsupportedUnaryOp");
  }
}

void BytecodeVisitor::Visit(Assign& node) {
  const auto& op = node.Kind();
  bool is_copy_assign = (&op == &optags::CopyAssign());

  if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableTarget())) {
    std::string object_type_name;
    if (auto* ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
      if (auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
        object_type_name = type_it->second;
      }
    }

    std::string expected_field_type_name;
    if (!object_type_name.empty()) {
      expected_field_type_name = GetFieldTypeName(object_type_name, field_access->Name());
    }
    if (expected_field_type_name.empty() && !current_class_name_.empty()) {
      expected_field_type_name = GetFieldTypeName(current_class_name_, field_access->Name());
    }

    node.MutableValue().Accept(*this);

    if (!expected_field_type_name.empty()) {
      std::string value_type_name = GetTypeNameForExpr(&node.MutableValue());
      EmitTypeConversionIfNeeded(expected_field_type_name, value_type_name);
    }

    if (is_copy_assign) {
      if (OperandType value_type = DetermineOperandType(&node.MutableValue());
          value_type == OperandType::kString || value_type == OperandType::kUnknown) {
      }
    }

    field_access->MutableObject().Accept(*this);

    int field_index = -1;
    if (!object_type_name.empty()) {
      field_index = FindFieldIndex(object_type_name, field_access->Name());
    }
    if (field_index < 0 && !current_class_name_.empty()) {
      field_index = FindFieldIndex(current_class_name_, field_access->Name());
    }
    if (field_index < 0) {
      field_index = 0;
    }
    EmitCommandWithInt("SetField", field_index);
    return;
  }

  if (is_copy_assign) {
    std::string target_type_name;

    if (auto* target_ident = dynamic_cast<IdentRef*>(&node.MutableTarget())) {
      if (auto type_it = variable_types_.find(target_ident->Name()); type_it != variable_types_.end()) {
        target_type_name = type_it->second;
      }
    }

    bool target_is_primitive = target_type_name == "int" || target_type_name == "float" || target_type_name == "byte" ||
                               target_type_name == "char" || target_type_name == "bool" ||
                               target_type_name == "pointer";

    if (!target_is_primitive && !target_type_name.empty()) {
      node.MutableValue().Accept(*this);

      std::string value_type_name = GetTypeNameForExpr(&node.MutableValue());

      node.MutableTarget().Accept(*this);

      std::string copy_method_name = GenerateCopyMethodId(target_type_name, value_type_name);
      EmitCommandWithStringWithoutBraces("Call", copy_method_name);
      return;
    }
  }

  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableTarget())) {
    // Check if this is a global variable before generating code
    bool is_global = static_variables_.contains(ident->Name());

    std::string expected_type_name;
    if (auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
      expected_type_name = type_it->second;
    }

    node.MutableValue().Accept(*this);

    if (!expected_type_name.empty()) {
      std::string value_type_name = GetTypeNameForExpr(&node.MutableValue());
      EmitTypeConversionIfNeeded(expected_type_name, value_type_name);
    }

    if (is_global) {
      EmitCommandWithInt("SetStatic", static_cast<int64_t>(GetStaticIndex(ident->Name())));
    } else {
      EmitCommandWithInt("SetLocal", static_cast<int64_t>(GetLocalIndex(ident->Name())));
    }
  } else if (auto* index_access = dynamic_cast<IndexAccess*>(&node.MutableTarget())) {
    node.MutableValue().Accept(*this);

    std::string array_type = GetTypeNameForExpr(&index_access->MutableObject());
    std::string elem_type = GetElementTypeForArray(array_type);
    std::string value_type_name = GetTypeNameForExpr(&node.MutableValue());
    EmitTypeConversionIfNeeded(elem_type, value_type_name);

    index_access->MutableIndexExpr().Accept(*this);
    index_access->MutableObject().Accept(*this);

    std::string method_name = GenerateArraySetAtMethodName(array_type);
    EmitCommandWithStringWithoutBraces("Call", method_name);
  } else {
    EmitCommand("UnsupportedAssignTarget");
  }
}

void BytecodeVisitor::Visit(Call& node) {
  auto& args = node.MutableArgs();

  if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&node.MutableCallee())) {
    std::string ns_name = ns_ref->Name();

    if (IsBuiltinSystemCommand(ns_name)) {
      EmitArgumentsInReverse(args);
      EmitCommand(ns_name);

      if (auto wrap_type_it = kBuiltinReturnPrimitives.find(ns_name); wrap_type_it != kBuiltinReturnPrimitives.end()) {
        if (std::string wrapper_type = wrap_type_it->second; wrapper_type == "Float") {
          EmitCommandWithStringWithoutBraces("CallConstructor", "_Float_float");
        } else if (wrapper_type == "Int") {
          EmitCommandWithStringWithoutBraces("CallConstructor", "_Int_int");
        }
      }

      return;
    }

    // Handle sys::ToString for primitive types
    if (ns_name == "ToString" && args.size() == 1) {
      // Get the type first (without emitting code)
      std::string arg_type = GetTypeNameForExpr(args[0].get());

      // Handle wrapper types by unwrapping first
      if (IsPrimitiveWrapper(arg_type)) {
        std::string primitive_type = GetPrimitiveTypeForWrapper(arg_type);
        // Now emit the argument code
        args[0]->Accept(*this);
        EmitCommand("Unwrap");
        arg_type = primitive_type;
      } else {
        // Emit the argument code
        args[0]->Accept(*this);
      }

      // For primitive types, use special instructions
      if (arg_type == "int") {
        EmitCommand("IntToString");
        return;
      }
      if (arg_type == "float") {
        EmitCommand("FloatToString");
        return;
      }
    }

    // Handle sys::ToInt for String type
    if (ns_name == "ToInt" && args.size() == 1) {
      // Get the type first (without emitting code)
      std::string arg_type = GetTypeNameForExpr(args[0].get());

      // For String type, use special instruction
      if (arg_type == "String") {
        // Now emit the argument code
        args[0]->Accept(*this);
        EmitCommand("StringToInt");
        return;
      }
    }

    // Handle sys::ToFloat for String type
    if (ns_name == "ToFloat" && args.size() == 1) {
      // Get the type first (without emitting code)
      std::string arg_type = GetTypeNameForExpr(args[0].get());

      // For String type, use special instruction
      if (arg_type == "String") {
        // Now emit the argument code
        args[0]->Accept(*this);
        EmitCommand("StringToFloat");
        return;
      }
    }

    // Handle sys::Sqrt for float type
    if (ns_name == "Sqrt" && args.size() == 1) {
      // Get the type first (without emitting code)
      std::string arg_type = GetTypeNameForExpr(args[0].get());

      // Handle wrapper types by unwrapping first
      if (IsPrimitiveWrapper(arg_type)) {
        std::string primitive_type = GetPrimitiveTypeForWrapper(arg_type);
        // Now emit the argument code
        args[0]->Accept(*this);
        EmitCommand("Unwrap");
        arg_type = primitive_type;
      } else {
        // Emit the argument code
        args[0]->Accept(*this);
      }

      // Sqrt only works with float type
      if (arg_type == "float") {
        EmitCommand("FloatSqrt");
        return;
      }
    }

    std::string full_name = "sys::" + ns_name;
    if (auto it = function_name_map_.find(full_name); it != function_name_map_.end()) {
      EmitArgumentsInReverse(args);
      EmitCommandWithStringWithoutBraces("Call", it->second);
      return;
    }

    EmitArgumentsInReverse(args);
    EmitCommandWithStringWithoutBraces("Call", full_name);
    return;
  }

  if (auto* ident = dynamic_cast<IdentRef*>(&node.MutableCallee())) {
    std::string name = ident->Name();

    if (kBuiltinTypeNames.contains(name)) {
      std::vector<std::string> expected_param_types;
      if (args.size() == 1) {
        std::string param_type;
        if (name == "Int") {
          param_type = "int";
        } else if (name == "Float") {
          param_type = "float";
        } else if (name == "String") {
          param_type = "String";
        } else if (name == "Bool") {
          param_type = "bool";
        } else if (name == "Char") {
          param_type = "char";
        } else if (name == "Byte") {
          param_type = "byte";
        } else {
          param_type = "Object";
        }
        expected_param_types.push_back(param_type);
      } else if (args.size() == 2) {
        expected_param_types.emplace_back("int");
        std::string type2;
        if (name == "IntArray") {
          type2 = "int";
        } else if (name == "FloatArray") {
          type2 = "float";
        } else if (name == "StringArray") {
          type2 = "String";
        } else if (name == "BoolArray") {
          type2 = "bool";
        } else if (name == "ByteArray") {
          type2 = "byte";
        } else if (name == "CharArray") {
          type2 = "char";
        } else {
          type2 = "Object";
        }
        expected_param_types.push_back(type2);
      } else if (args.size() == 3) {
        expected_param_types.emplace_back("float");
        expected_param_types.emplace_back("float");
        expected_param_types.emplace_back("float");
      }

      for (size_t i = args.size(); i > 0; --i) {
        size_t arg_idx = i - 1;
        Expr* arg = args[arg_idx].get();

        arg->Accept(*this);

        if (arg_idx < expected_param_types.size()) {
          const std::string& expected_type_name = expected_param_types[arg_idx];
          std::string arg_type_name = GetTypeNameForExpr(arg);

          bool needs_wrap = IsPrimitiveWrapper(expected_type_name) && IsPrimitiveType(arg_type_name);
          bool needs_unwrap = IsPrimitiveType(expected_type_name) && IsPrimitiveWrapper(arg_type_name);

          if (needs_wrap) {
            const std::string& wrapper_type = expected_type_name;
            const std::string& primitive_type = arg_type_name;
            std::string constructor_name = std::string("_" + wrapper_type + "_").append(primitive_type);
            EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
          } else if (needs_unwrap) {
            EmitCommand("Unwrap");
          }
        }
      }

      std::string constructor_name = "_" + name;
      if (args.size() == 1) {
        constructor_name.append("_" + expected_param_types[0]);
      } else if (args.size() == 2) {
        constructor_name.append("_" + expected_param_types[0] + "_" + expected_param_types[1]);
      } else if (args.size() == 3) {
        constructor_name.append("_float_float_float");
      }
      EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
      return;
    }

    std::string ctor_key = name + "::<ctor>";
    if (auto ctor_it = method_name_map_.find(ctor_key); ctor_it != method_name_map_.end()) {
      if (auto param_types_it = constructor_params_.find(ctor_key); param_types_it != constructor_params_.end()) {
        const auto& expected_param_types = param_types_it->second;

        for (size_t i = args.size(); i > 0; --i) {
          size_t arg_idx = i - 1;
          Expr* arg = args[arg_idx].get();

          arg->Accept(*this);

          if (arg_idx < expected_param_types.size()) {
            const TypeReference& expected_type = expected_param_types[arg_idx];
            std::string expected_type_name = TypeToMangledName(expected_type);
            std::string arg_type_name = GetTypeNameForExpr(arg);

            bool needs_wrap = IsPrimitiveWrapper(expected_type_name) && IsPrimitiveType(arg_type_name);
            bool needs_unwrap = IsPrimitiveType(expected_type_name) && IsPrimitiveWrapper(arg_type_name);

            if (needs_wrap) {
              const std::string& wrapper_type = expected_type_name;
              const std::string& primitive_type = arg_type_name;
              std::string constructor_name = "_";
              constructor_name.append(wrapper_type + "_");
              constructor_name.append(primitive_type);
              EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
            } else if (needs_unwrap) {
              EmitCommand("Unwrap");
            }
          }
        }
      } else {
        for (auto& arg : std::ranges::reverse_view(args)) {
          arg->Accept(*this);
        }
      }
      EmitCommandWithStringWithoutBraces("CallConstructor", ctor_it->second);
      return;
    }

    // Handle ToString for primitive types
    if (name == "ToString" && args.size() == 1) {
      args[0]->Accept(*this);
      std::string arg_type = GetTypeNameForExpr(args[0].get());

      // For wrapper types, use their ToString method from kBuiltinMethods
      if (kBuiltinTypeNames.contains(arg_type)) {
        if (auto methods_it = kBuiltinMethods.find(arg_type); methods_it != kBuiltinMethods.end()) {
          if (auto tostring_it = methods_it->second.find("ToString"); tostring_it != methods_it->second.end()) {
            EmitCommandWithStringWithoutBraces("Call", tostring_it->second);
            return;
          }
        }
      }

      // For primitive types, use special instructions
      if (arg_type == "int") {
        EmitCommand("IntToString");
        return;
      }
      if (arg_type == "float") {
        EmitCommand("FloatToString");
        return;
      }
      if (arg_type == "byte") {
        EmitCommand("ByteToString");
        return;
      }
      if (arg_type == "char") {
        EmitCommand("CharToString");
        return;
      }
      if (arg_type == "bool") {
        EmitCommand("BoolToString");
        return;
      }
    }

    // Try to resolve overload
    std::string resolved_mangled = ResolveFunctionOverload(name, args);
    if (!resolved_mangled.empty()) {
      for (auto& arg : std::ranges::reverse_view(args)) {
        arg->Accept(*this);
      }
      EmitCommandWithStringWithoutBraces("Call", resolved_mangled);
      return;
    }

    // Fallback to old behavior for backward compatibility
    if (auto it = function_name_map_.find(name); it != function_name_map_.end()) {
      for (auto& arg : std::ranges::reverse_view(args)) {
        arg->Accept(*this);
      }
      EmitCommandWithStringWithoutBraces("Call", it->second);
      return;
    }

    for (auto& arg : std::ranges::reverse_view(args)) {
      arg->Accept(*this);
    }
    EmitCommandWithStringWithoutBraces("Call", name);
    return;
  }

  if (auto* field_access = dynamic_cast<FieldAccess*>(&node.MutableCallee())) {
    std::string method_name = field_access->Name();

    std::string object_type;
    if (auto* ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
      if (auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
        object_type = type_it->second;
      }
    } else if (dynamic_cast<StringLit*>(&field_access->MutableObject())) {
      object_type = "String";
    } else if (dynamic_cast<IntLit*>(&field_access->MutableObject())) {
      object_type = "int";
    } else if (dynamic_cast<FloatLit*>(&field_access->MutableObject())) {
      object_type = "float";
    } else if (dynamic_cast<ThisExpr*>(&field_access->MutableObject())) {
      object_type = current_class_name_;
    }

    if (object_type.empty()) {
      object_type = GetTypeNameForExpr(&field_access->MutableObject());
    }

    if (auto* nested_field_access = dynamic_cast<FieldAccess*>(&field_access->MutableObject())) {
      std::string field_type;
      std::string nested_object_type = object_type;
      if (dynamic_cast<ThisExpr*>(&nested_field_access->MutableObject())) {
        nested_object_type = current_class_name_;
      } else if (auto* nested_ident = dynamic_cast<IdentRef*>(&nested_field_access->MutableObject())) {
        if (auto type_it = variable_types_.find(nested_ident->Name()); type_it != variable_types_.end()) {
          nested_object_type = type_it->second;
        }
      }

      if (nested_object_type.empty() && !current_class_name_.empty()) {
        nested_object_type = current_class_name_;
      }

      if (!nested_object_type.empty()) {
        if (auto fields_it = class_fields_.find(nested_object_type); fields_it != class_fields_.end()) {
          for (const auto& fields = fields_it->second; const auto& [name, type] : fields) {
            if (name == nested_field_access->Name()) {
              field_type = TypeToMangledName(type);
              break;
            }
          }
        }
      }

      if (field_type.empty() && !current_class_name_.empty()) {
        if (auto fields_it = class_fields_.find(current_class_name_); fields_it != class_fields_.end()) {
          for (const auto& fields = fields_it->second; const auto& [name, type] : fields) {
            if (name == nested_field_access->Name()) {
              field_type = TypeToMangledName(type);
              break;
            }
          }
        }
      }

      if (!field_type.empty()) {
        object_type = field_type;
      }
    }

    if (kPrimitiveTypeNames.contains(object_type)) {
      if (method_name == "ToString") {
        if (object_type == "int") {
          EmitCommand("IntToString");
          return;
        }
        if (object_type == "float") {
          EmitCommand("FloatToString");
          return;
        }
      }
    }

    if (kBuiltinTypeNames.contains(object_type)) {
      auto generate_array_method_name = [&](const std::string& array_type,
                                            const std::string& code_method_name) -> std::string {
        std::string base_name = "_" + array_type + "_" + code_method_name;

        std::string elem_type;
        if (array_type == "IntArray") {
          elem_type = "int";
        } else if (array_type == "FloatArray") {
          elem_type = "float";
        } else if (array_type == "ByteArray") {
          elem_type = "byte";
        } else if (array_type == "BoolArray") {
          elem_type = "bool";
        } else if (array_type == "CharArray") {
          elem_type = "char";
        } else {
          elem_type = "Object";
        }

        if (code_method_name == "Length" || code_method_name == "GetHash" || code_method_name == "ToString" ||
            code_method_name == "Equals" || code_method_name == "Capacity" || code_method_name == "GetAt") {
          base_name += "_<C>";
        } else {
          base_name += "_<M>";
        }

        if (code_method_name == "Add") {
          base_name += "_" + elem_type;
        } else if (code_method_name == "GetAt" || code_method_name == "RemoveAt" || code_method_name == "Reserve") {
          base_name += "_int";
        } else if (code_method_name == "SetAt" || code_method_name == "InsertAt") {
          base_name += "_int_" + elem_type;
        } else if (code_method_name == "Equals") {
          base_name += "_Object";
        }

        return base_name;
      };

      std::string method_call;
      if (!object_type.empty()) {
        if (auto type_it = kBuiltinMethods.find(object_type); type_it != kBuiltinMethods.end()) {
          if (auto method_it = type_it->second.find(method_name); method_it != type_it->second.end()) {
            method_call = method_it->second;
          } else {
            if (object_type.find("Array") != std::string::npos) {
              method_call = generate_array_method_name(object_type, method_name);
            }
          }
        }

        // For builtin types, if method not found in kBuiltinMethods, generate direct Call name
        if (method_call.empty() && kBuiltinTypeNames.contains(object_type)) {
          // Generate method name: _TypeName_MethodName_<C> or _TypeName_MethodName_<M>
          // Default to <C> for const methods, can be adjusted if needed
          method_call = "_" + object_type + "_" + method_name + "_<C>";
          // Add parameter types if there are arguments
          if (!args.empty()) {
            // For now, assume Object type for all parameters (this matches most cases)
            // More sophisticated type inference could be added later
            for (size_t i = 0; i < args.size(); ++i) {
              method_call += "_Object";
            }
          }
        }
      }

      if (!method_call.empty()) {
        std::vector<std::string> expected_param_types;

        if (method_name == "Add") {
          std::string elem_type;
          if (object_type == "IntArray") {
            elem_type = "int";
          } else if (object_type == "FloatArray") {
            elem_type = "float";
          } else if (object_type == "ByteArray") {
            elem_type = "byte";
          } else if (object_type == "BoolArray") {
            elem_type = "bool";
          } else if (object_type == "CharArray") {
            elem_type = "char";
          } else {
            elem_type = "Object";
          }
          expected_param_types.push_back(elem_type);
        } else if (method_name == "SetAt" || method_name == "InsertAt") {
          expected_param_types.emplace_back("int");
          std::string elem_type;
          if (object_type == "IntArray") {
            elem_type = "int";
          } else if (object_type == "FloatArray") {
            elem_type = "float";
          } else if (object_type == "ByteArray") {
            elem_type = "byte";
          } else if (object_type == "BoolArray") {
            elem_type = "bool";
          } else if (object_type == "CharArray") {
            elem_type = "char";
          } else {
            elem_type = "Object";
          }
          expected_param_types.push_back(elem_type);
        } else if (method_name == "Equals") {
          expected_param_types.emplace_back("Object");
        } else if (method_name == "Substring") {
          expected_param_types.emplace_back("int");
          expected_param_types.emplace_back("int");
        } else if (method_name == "Compare") {
          expected_param_types.emplace_back("String");
        } else if (method_name == "Reserve" || method_name == "RemoveAt") {
          expected_param_types.emplace_back("int");
        }

        // For regular Call: emit arguments first (right to left), then object
        for (size_t i = args.size(); i > 0; --i) {
          size_t arg_idx = i - 1;
          Expr* arg = args[arg_idx].get();

          arg->Accept(*this);

          if (size_t param_idx = args.size() - 1 - arg_idx; param_idx < expected_param_types.size()) {
            const std::string& expected_type_name = expected_param_types[param_idx];
            std::string arg_type_name = GetTypeNameForExpr(arg);

            bool needs_wrap = IsPrimitiveWrapper(expected_type_name) && IsPrimitiveType(arg_type_name);
            bool needs_unwrap = IsPrimitiveType(expected_type_name) && IsPrimitiveWrapper(arg_type_name);

            if (needs_wrap) {
              const std::string& wrapper_type = expected_type_name;
              const std::string& primitive_type = arg_type_name;
              std::string constructor_name = "_" + wrapper_type;
              constructor_name.append("_" + primitive_type);
              EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
            } else if (needs_unwrap) {
              EmitCommand("Unwrap");
            }
          }
        }
        field_access->MutableObject().Accept(*this);

        EmitCommandWithStringWithoutBraces("Call", method_call);
        return;
      }

      // For CallVirtual: emit arguments first (right to left), then object (leftmost)
      for (auto& arg : std::ranges::reverse_view(args)) {
        arg->Accept(*this);
      }
      field_access->MutableObject().Accept(*this);
      std::string vtable_name = "_" + method_name + "_<C>";
      if (!args.empty()) {
        vtable_name += "_Object";
      }
      EmitCommandWithStringWithoutBraces("CallVirtual", vtable_name);
      return;
    }

    int method_count = 0;
    std::string full_method_name;
    std::string vtable_name;
    std::string specific_method_name;

    bool is_interface_type = false;
    if (object_type == "Object") {
      is_interface_type = true;
    } else if (!object_type.empty() && object_type.size() > 1 && object_type[0] == 'I') {
      if (std::isupper(static_cast<unsigned char>(object_type[1]))) {
        if (!method_name_map_.contains(object_type + "::" + method_name)) {
          is_interface_type = true;
        }
      }
    }

    if (!object_type.empty()) {
      if (auto specific_it = method_name_map_.find(object_type + "::" + method_name);
          specific_it != method_name_map_.end()) {
        specific_method_name = specific_it->second;
      }
    }

    for (const auto& [fst, snd] : method_name_map_) {
      if (auto pos = fst.rfind("::"); pos != std::string::npos) {
        if (std::string code_method_name = fst.substr(pos + 2); code_method_name == method_name) {
          method_count++;
          if (full_method_name.empty()) {
            full_method_name = snd;
          }

          if (auto vtable_it = method_vtable_map_.find(fst);
              vtable_it != method_vtable_map_.end() && vtable_name.empty()) {
            vtable_name = vtable_it->second;
          }
        }
      }
    }

    bool use_virtual = method_count > 1 || is_interface_type ||
                       (!object_type.empty() && method_count > 0 && specific_method_name.empty());

    for (auto& arg : std::ranges::reverse_view(args)) {
      arg->Accept(*this);
    }

    field_access->MutableObject().Accept(*this);

    if (!specific_method_name.empty()) {
      EmitCommandWithStringWithoutBraces("Call", specific_method_name);
    } else if (use_virtual) {
      if (vtable_name.empty()) {
        bool is_mutable = false;
        vtable_name = GenerateMethodVTableName(method_name, std::vector<Param>(), is_mutable);
      }
      EmitCommandWithStringWithoutBraces("CallVirtual", vtable_name);
    } else if (!full_method_name.empty()) {
      EmitCommandWithStringWithoutBraces("Call", full_method_name);
    } else {
      std::string vtable_method_name = "_" + method_name + "_<C>";
      if (!args.empty()) {
        vtable_method_name += "_Object";
      }
      EmitCommandWithStringWithoutBraces("CallVirtual", vtable_method_name);
    }
    return;
  }

  node.MutableCallee().Accept(*this);
  EmitCommand("CallDynamic");
}

void BytecodeVisitor::Visit(FieldAccess& node) {
  node.MutableObject().Accept(*this);

  std::string object_type_name;
  if (const auto* ident = dynamic_cast<IdentRef*>(&node.MutableObject())) {
    if (const auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
      object_type_name = type_it->second;
    }
  }

  int field_index = -1;
  if (!object_type_name.empty()) {
    field_index = FindFieldIndex(object_type_name, node.Name());
  }
  if (field_index < 0 && !current_class_name_.empty()) {
    field_index = FindFieldIndex(current_class_name_, node.Name());
  }
  if (field_index < 0) {
    field_index = 0;
  }

  EmitCommandWithInt("GetField", field_index);
}

void BytecodeVisitor::Visit(IndexAccess& node) {
  node.MutableIndexExpr().Accept(*this);
  node.MutableObject().Accept(*this);

  std::string array_type = GetTypeNameForExpr(&node.MutableObject());
  std::string method_name = GenerateArrayGetAtMethodName(array_type);
  EmitCommandWithStringWithoutBraces("Call", method_name);
}

void BytecodeVisitor::Visit(NamespaceRef& node) {
  EmitCommandWithInt("LoadStatic", static_cast<int64_t>(GetStaticIndex(node.Name())));
}

void BytecodeVisitor::Visit(SafeCall& node) {
  node.MutableObject().Accept(*this);
  for (auto& it : std::ranges::reverse_view(node.MutableArgs())) {
    it->Accept(*this);
  }
  EmitCommandWithStringWithoutBraces("SafeCall", node.Method());
}

void BytecodeVisitor::Visit(Elvis& node) {
  bool use_direct_var = false;
  size_t lhs_var_index = 0;
  if (const auto* ident = dynamic_cast<IdentRef*>(&node.MutableLhs())) {
    const auto var_it = variable_types_.find(ident->Name());
    if (const auto local_it = local_variables_.find(ident->Name());
        var_it != variable_types_.end() && local_it != local_variables_.end()) {
      use_direct_var = true;
      lhs_var_index = local_it->second;
    }
  }

  size_t temp_var_index = 0;
  if (!use_direct_var) {
    node.MutableLhs().Accept(*this);
    std::string temp_var_name = "__elvis_temp_" + std::to_string(next_local_index_);
    temp_var_index = GetLocalIndex(temp_var_name);
    EmitCommandWithInt("SetLocal", static_cast<int64_t>(temp_var_index));
  }

  size_t var_index = use_direct_var ? lhs_var_index : temp_var_index;

  node.MutableRhs().Accept(*this);
  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(var_index));
  EmitCommand("NullCoalesce");
}

void BytecodeVisitor::Visit(CastAs& node) {
  OperandType source_type = DetermineOperandType(&node.MutableExpression());
  std::string target_type_name = TypeToMangledName(node.Type());

  if (source_type == OperandType::kInt && (target_type_name == "float" || target_type_name == "Float")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("IntToFloat");
    return;
  }

  if (source_type == OperandType::kFloat && (target_type_name == "int" || target_type_name == "Int")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("FloatToInt");
    return;
  }

  if (source_type == OperandType::kByte && (target_type_name == "int" || target_type_name == "Int")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("ByteToInt");
    return;
  }

  if (source_type == OperandType::kInt && (target_type_name == "byte" || target_type_name == "Byte")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("IntToByte");
    return;
  }

  if (source_type == OperandType::kString && (target_type_name == "int" || target_type_name == "Int")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("StringToInt");
    return;
  }

  if (source_type == OperandType::kString && (target_type_name == "float" || target_type_name == "Float")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("StringToFloat");
    return;
  }

  if (source_type == OperandType::kInt && target_type_name == "String") {
    node.MutableExpression().Accept(*this);
    EmitCommand("IntToString");
    return;
  }

  if (source_type == OperandType::kFloat && target_type_name == "String") {
    node.MutableExpression().Accept(*this);
    EmitCommand("FloatToString");
    return;
  }

  if (source_type == OperandType::kChar && (target_type_name == "byte" || target_type_name == "Byte")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("CharToByte");
    return;
  }

  if (source_type == OperandType::kByte && (target_type_name == "char" || target_type_name == "Char")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("ByteToChar");
    return;
  }

  if (source_type == OperandType::kBool && (target_type_name == "byte" || target_type_name == "Byte")) {
    node.MutableExpression().Accept(*this);
    EmitCommand("BoolToByte");
    return;
  }

  bool use_direct_var = false;
  size_t expr_var_index = 0;
  if (const auto* ident = dynamic_cast<IdentRef*>(&node.MutableExpression())) {
    const auto var_it = variable_types_.find(ident->Name());
    if (const auto local_it = local_variables_.find(ident->Name());
        var_it != variable_types_.end() && local_it != local_variables_.end()) {
      use_direct_var = true;
      expr_var_index = local_it->second;
    }
  }

  size_t temp_var_index = 0;
  if (!use_direct_var) {
    node.MutableExpression().Accept(*this);
    std::string temp_var_name = "__cast_temp_" + std::to_string(next_local_index_);
    temp_var_index = GetLocalIndex(temp_var_name);
    EmitCommandWithInt("SetLocal", static_cast<int64_t>(temp_var_index));
  }

  size_t var_index = use_direct_var ? expr_var_index : temp_var_index;

  const TypeReference base_type = node.Type().WithoutNullable();
  std::string base_type_name = TypeToMangledName(base_type);

  EmitIndent();
  output_ << "if ";
  EmitBlockStartWithoutSpaces();

  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(var_index));
  EmitCommandWithStringWithoutBraces("IsType", base_type_name);

  EmitBlockEndWithoutEscape();
  output_ << " then ";
  EmitBlockStartWithoutSpaces();

  EmitCommandWithInt("LoadLocal", static_cast<int64_t>(var_index));
  EmitCommandWithStringWithoutBraces("CallConstructor", "_Nullable_Object");

  EmitBlockEndWithoutEscape();
  output_ << " else ";
  EmitBlockStartWithoutSpaces();

  EmitCommand("PushNull");

  EmitBlockEnd();
}

void BytecodeVisitor::Visit(TypeTestIs& node) {
  node.MutableExpression().Accept(*this);
  std::string type_name = TypeToMangledName(node.Type());
  EmitCommandWithStringWithoutBraces("IsType", type_name);
}

void BytecodeVisitor::Visit(IdentRef& node) {
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
  EmitCommandWithInt("PushChar", node.Value());
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
  return "_" + class_name + "_copy_<M>_" + param_type;
}

std::string BytecodeVisitor::GenerateMethodVTableName(const std::string& method_name,
                                                      const std::vector<Param>& params,
                                                      bool is_mutable) {
  std::ostringstream oss;

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
  TypeReference resolved_type = type;

  if (!type.QualifiedName().empty()) {
    std::string_view type_name = type.SimpleName();
    auto alias_it = type_aliases_.find(std::string(type_name));
    if (alias_it != type_aliases_.end()) {
      resolved_type = alias_it->second;
      if (type.IsNullable()) {
        resolved_type.MakeNullable();
      }
    }
  }

  std::ostringstream oss;

  if (const auto& qname = resolved_type.QualifiedName(); !qname.empty()) {
    oss << qname.back();
  }

  if (resolved_type.Arity() > 0) {
    oss << "<";
    for (size_t i = 0; i < resolved_type.Arity(); ++i) {
      if (i) {
        oss << ",";
      }
      oss << TypeToMangledName(resolved_type.TypeArguments()[i]);
    }
    oss << ">";
  }

  if (resolved_type.IsNullable()) {
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
  if (!local_variables_.contains(name)) {
    local_variables_[name] = next_local_index_++;
  }
  return local_variables_[name];
}

size_t BytecodeVisitor::GetStaticIndex(const std::string& name) {
  if (!static_variables_.contains(name)) {
    static_variables_[name] = next_static_index_++;
  }
  return static_variables_[name];
}

void BytecodeVisitor::ResetLocalVariables() {
  // Save global variable types before clearing
  std::unordered_map<std::string, std::string> global_types;
  for (const auto& [name, type] : variable_types_) {
    if (static_variables_.contains(name)) {
      global_types[name] = type;
    }
  }

  local_variables_.clear();
  variable_types_.clear();
  next_local_index_ = 0;

  // Restore global variable types
  for (const auto& [name, type] : global_types) {
    variable_types_[name] = type;
  }
}

BytecodeVisitor::OperandType BytecodeVisitor::DetermineOperandType(Expr* expr) {
  if (expr == nullptr) {
    return OperandType::kUnknown;
  }

  if (dynamic_cast<IntLit*>(expr) != nullptr) {
    return OperandType::kInt;
  }
  if (dynamic_cast<FloatLit*>(expr) != nullptr) {
    return OperandType::kFloat;
  }
  if (dynamic_cast<StringLit*>(expr) != nullptr) {
    return OperandType::kString;
  }
  if (dynamic_cast<BoolLit*>(expr) != nullptr) {
    return OperandType::kBool;
  }
  if (dynamic_cast<CharLit*>(expr) != nullptr) {
    return OperandType::kChar;
  }

  if (const auto* ident = dynamic_cast<IdentRef*>(expr)) {
    if (const auto it = variable_types_.find(ident->Name()); it != variable_types_.end()) {
      const std::string& type_name = it->second;
      if (type_name == "int" || type_name == "Int") {
        return OperandType::kInt;
      }
      if (type_name == "float" || type_name == "Float") {
        return OperandType::kFloat;
      }
      if (type_name == "byte" || type_name == "Byte") {
        return OperandType::kByte;
      }
      if (type_name == "bool" || type_name == "Bool") {
        return OperandType::kBool;
      }
      if (type_name == "char" || type_name == "Char") {
        return OperandType::kChar;
      }
      if (type_name == "String") {
        return OperandType::kString;
      }
    }
  }

  if (auto* field_access = dynamic_cast<FieldAccess*>(expr)) {
    // First, try to get the object type using GetTypeNameForExpr
    // This handles cases where the object is a variable, method call result, etc.
    std::string object_type_name = GetTypeNameForExpr(&field_access->MutableObject());
    
    // If object_type is "unknown" (from a Call expression), try to resolve it for chained calls
    if (object_type_name == "unknown" || object_type_name.empty()) {
      if (auto* nested_call = dynamic_cast<Call*>(&field_access->MutableObject())) {
        // Get the return type of the nested call by examining its structure
        if (auto* nested_field_access = dynamic_cast<FieldAccess*>(&nested_call->MutableCallee())) {
          std::string nested_method_name = nested_field_access->Name();
          std::string nested_object_type = GetTypeNameForExpr(&nested_field_access->MutableObject());
          
          // Check if this is a method call on a user-defined type
          if (!nested_object_type.empty() && !kBuiltinTypeNames.contains(nested_object_type)) {
            std::string nested_method_key = nested_object_type + "::" + nested_method_name;
            if (const auto nested_it = method_return_types_.find(nested_method_key); nested_it != method_return_types_.end()) {
              object_type_name = nested_it->second;
            }
          }
        }
      }
      
      // If that didn't work, try checking if it's a direct IdentRef
      if (object_type_name.empty() || object_type_name == "unknown") {
        if (auto* ident = dynamic_cast<IdentRef*>(&field_access->MutableObject())) {
          if (const auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
            object_type_name = type_it->second;
          }
        }
      }
    }

    // If we have an object type, look up the field in that class
    if (!object_type_name.empty() && object_type_name != "unknown") {
      if (const auto fields_it = class_fields_.find(object_type_name); fields_it != class_fields_.end()) {
        for (const auto& fields = fields_it->second; const auto& [fst, snd] : fields) {
          if (fst == field_access->Name()) {
            const std::string& type_name = TypeToMangledName(snd);
            if (type_name == "int" || type_name == "Int") {
              return OperandType::kInt;
            }
            if (type_name == "float" || type_name == "Float") {
              return OperandType::kFloat;
            }
            if (type_name == "byte" || type_name == "Byte") {
              return OperandType::kByte;
            }
            if (type_name == "bool" || type_name == "Bool") {
              return OperandType::kBool;
            }
            if (type_name == "char" || type_name == "Char") {
              return OperandType::kChar;
            }
            if (type_name == "String") {
              return OperandType::kString;
            }
            break;
          }
        }
      }
    }

    // Fallback: check current class if we're inside a class
    if (!current_class_name_.empty()) {
      if (const auto fields_it = class_fields_.find(current_class_name_); fields_it != class_fields_.end()) {
        for (const auto& fields = fields_it->second; const auto& [fst, snd] : fields) {
          if (fst == field_access->Name()) {
            const std::string& type_name = TypeToMangledName(snd);
            if (type_name == "int" || type_name == "Int") {
              return OperandType::kInt;
            }
            if (type_name == "float" || type_name == "Float") {
              return OperandType::kFloat;
            }
            if (type_name == "byte" || type_name == "Byte") {
              return OperandType::kByte;
            }
            if (type_name == "bool" || type_name == "Bool") {
              return OperandType::kBool;
            }
            if (type_name == "char" || type_name == "Char") {
              return OperandType::kChar;
            }
            if (type_name == "String") {
              return OperandType::kString;
            }
            break;
          }
        }
      }
    }
  }

  if (const auto* cast = dynamic_cast<CastAs*>(expr)) {
    const std::string& type_name = TypeToMangledName(cast->Type());
    if (type_name == "int" || type_name == "Int") {
      return OperandType::kInt;
    }
    if (type_name == "float" || type_name == "Float") {
      return OperandType::kFloat;
    }
    if (type_name == "byte" || type_name == "Byte") {
      return OperandType::kByte;
    }
    if (type_name == "bool" || type_name == "Bool") {
      return OperandType::kBool;
    }
    if (type_name == "char" || type_name == "Char") {
      return OperandType::kChar;
    }
    if (type_name == "String") {
      return OperandType::kString;
    }
  }

  if (auto* binary = dynamic_cast<Binary*>(expr)) {
    OperandType lhs_type = DetermineOperandType(&binary->MutableLhs());
    OperandType rhs_type = DetermineOperandType(&binary->MutableRhs());

    const auto& op = binary->Op();
    if (&op == &optags::LeftShift() || &op == &optags::RightShift()) {
      return lhs_type != OperandType::kUnknown ? lhs_type : OperandType::kInt;
    }

    if (const bool is_bitwise = &op == &optags::BitwiseAnd() || &op == &optags::BitwiseOr() || &op == &optags::Xor();
        is_bitwise && (lhs_type == OperandType::kByte || rhs_type == OperandType::kByte)) {
      return OperandType::kByte;
    }

    if (lhs_type == OperandType::kFloat || rhs_type == OperandType::kFloat) {
      return OperandType::kFloat;
    }

    if (lhs_type == OperandType::kInt || rhs_type == OperandType::kInt) {
      return OperandType::kInt;
    }

    if (lhs_type == OperandType::kByte || rhs_type == OperandType::kByte) {
      return OperandType::kByte;
    }

    if (lhs_type == OperandType::kString || rhs_type == OperandType::kString) {
      return OperandType::kString;
    }

    if (lhs_type != OperandType::kUnknown) {
      return lhs_type;
    }

    if (rhs_type != OperandType::kUnknown) {
      return rhs_type;
    }
  }

  if (auto* unary = dynamic_cast<Unary*>(expr)) {
    return DetermineOperandType(&unary->MutableOperand());
  }

  if (auto* call = dynamic_cast<Call*>(expr)) {
    // Use GetTypeNameForExpr to determine return type
    std::string return_type = GetTypeNameForExpr(call);
    if (return_type == "int") {
      return OperandType::kInt;
    }
    if (return_type == "float") {
      return OperandType::kFloat;
    }
    if (return_type == "byte") {
      return OperandType::kByte;
    }
    if (return_type == "bool") {
      return OperandType::kBool;
    }
    if (return_type == "char") {
      return OperandType::kChar;
    }
    if (return_type == "String") {
      return OperandType::kString;
    }
  }

  return OperandType::kUnknown;
}

std::string BytecodeVisitor::GetOperandTypeName(Expr* expr) {
  switch (DetermineOperandType(expr)) {
    case OperandType::kInt:
      return "int";
    case OperandType::kFloat:
      return "float";
    case OperandType::kByte:
      return "byte";
    case OperandType::kBool:
      return "bool";
    case OperandType::kString:
      return "String";
    default:
      return "unknown";
  }
}

std::string BytecodeVisitor::GetTypeNameForExpr(Expr* expr) {
  if (expr == nullptr) {
    return "unknown";
  }

  if (const auto* ident = dynamic_cast<IdentRef*>(expr)) {
    if (const auto it = variable_types_.find(ident->Name()); it != variable_types_.end()) {
      return it->second;
    }
  }

  if (auto* field = dynamic_cast<FieldAccess*>(expr)) {
    // First, try to get the object type recursively using GetTypeNameForExpr
    // This handles cases where the object is a variable, method call result, etc.
    std::string object_type_name = GetTypeNameForExpr(&field->MutableObject());
    
    // If object_type is "unknown" (from a Call expression), try to resolve it for chained calls
    if (object_type_name == "unknown" || object_type_name.empty()) {
      if (auto* nested_call = dynamic_cast<Call*>(&field->MutableObject())) {
        // Get the return type of the nested call by examining its structure
        if (auto* nested_field_access = dynamic_cast<FieldAccess*>(&nested_call->MutableCallee())) {
          std::string nested_method_name = nested_field_access->Name();
          std::string nested_object_type = GetTypeNameForExpr(&nested_field_access->MutableObject());
          
          // Check if this is a method call on a user-defined type
          if (!nested_object_type.empty() && !kBuiltinTypeNames.contains(nested_object_type)) {
            std::string nested_method_key = nested_object_type + "::" + nested_method_name;
            if (const auto nested_it = method_return_types_.find(nested_method_key); nested_it != method_return_types_.end()) {
              object_type_name = nested_it->second;
            }
          }
        }
      }
      
      // If that didn't work, try checking if it's a direct IdentRef
      if (object_type_name.empty() || object_type_name == "unknown") {
        if (const auto* ident = dynamic_cast<IdentRef*>(&field->MutableObject())) {
          if (const auto type_it = variable_types_.find(ident->Name()); type_it != variable_types_.end()) {
            object_type_name = type_it->second;
          }
        }
      }
    }

    // If we have an object type, look up the field in that class
    if (!object_type_name.empty() && object_type_name != "unknown") {
      if (const auto fields_it = class_fields_.find(object_type_name); fields_it != class_fields_.end()) {
        for (const auto& [fst, snd] : fields_it->second) {
          if (fst == field->Name()) {
            return TypeToMangledName(snd);
          }
        }
      }
    }

    // Fallback: check current class if we're inside a class
    if (!current_class_name_.empty()) {
      if (const auto fields_it = class_fields_.find(current_class_name_); fields_it != class_fields_.end()) {
        for (const auto& [fst, snd] : fields_it->second) {
          if (fst == field->Name()) {
            return TypeToMangledName(snd);
          }
        }
      }
    }
  }

  if (dynamic_cast<IntLit*>(expr) != nullptr) {
    return "int";
  }
  if (dynamic_cast<FloatLit*>(expr) != nullptr) {
    return "float";
  }
  if (dynamic_cast<ByteLit*>(expr) != nullptr) {
    return "byte";
  }
  if (dynamic_cast<CharLit*>(expr) != nullptr) {
    return "char";
  }
  if (dynamic_cast<BoolLit*>(expr) != nullptr) {
    return "bool";
  }
  if (dynamic_cast<StringLit*>(expr) != nullptr) {
    return "String";
  }

  if (auto* index_access = dynamic_cast<IndexAccess*>(expr)) {
    std::string array_type = GetTypeNameForExpr(&index_access->MutableObject());
    return GetElementTypeForArray(array_type);
  }

  if (auto* call = dynamic_cast<Call*>(expr)) {
    if (const auto* ident = dynamic_cast<IdentRef*>(&call->MutableCallee())) {
      std::string func_name = ident->Name();
      // Handle constructor calls for builtin wrapper types
      if (kBuiltinTypeNames.contains(func_name)) {
        return func_name; // Int(0) returns "Int", Float(1.0) returns "Float", etc.
      }
      if (const auto it = function_return_types_.find(func_name); it != function_return_types_.end()) {
        return it->second;
      }
    }

    // Handle sys::Sqrt and other sys namespace functions
    if (auto* ns_ref = dynamic_cast<NamespaceRef*>(&call->MutableCallee())) {
      std::string ns_name = ns_ref->Name();
      // Check if namespace is "sys" by examining NamespaceExpr
      if (const auto* ns_ident = dynamic_cast<IdentRef*>(&ns_ref->MutableNamespaceExpr())) {
        if (ns_ident->Name() == "sys") {
          // Check for built-in return types
          if (const auto it = kBuiltinReturnPrimitives.find(ns_name); it != kBuiltinReturnPrimitives.end()) {
            return it->second;
          }
          if (ns_name == "Sqrt" && call->Args().size() == 1) {
            return "float";
          }
          if (ns_name == "ToString" && call->Args().size() == 1) {
            // ToString returns String, but we need to check the argument type
            // to determine which *ToString instruction to use
            std::string arg_type = GetTypeNameForExpr(call->Args()[0].get());
            if (arg_type == "int" || arg_type == "Int") {
              return "String";
            }
            if (arg_type == "float" || arg_type == "Float") {
              return "String";
            }
            // For other types, ToString still returns String
            return "String";
          }
          if (ns_name == "ToInt" && call->Args().size() == 1) {
            return "int";
          }
          if (ns_name == "ToFloat" && call->Args().size() == 1) {
            return "float";
          }
          if (ns_name == "GetOsName" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "ReadLine" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "ReadChar" && call->Args().size() == 0) {
            return "Char";
          }
          // Time and Date Operations
          if (ns_name == "FormatDateTime" && call->Args().size() == 2) {
            return "String";
          }
          if (ns_name == "ParseDateTime" && call->Args().size() == 2) {
            return "Int";
          }
          // File Operations
          if (ns_name == "FileExists" && call->Args().size() == 1) {
            return "Bool";
          }
          if (ns_name == "DirectoryExists" && call->Args().size() == 1) {
            return "Bool";
          }
          if (ns_name == "CreateDirectory" && call->Args().size() == 1) {
            return "Bool";
          }
          if (ns_name == "DeleteFile" && call->Args().size() == 1) {
            return "Bool";
          }
          if (ns_name == "DeleteDirectory" && call->Args().size() == 1) {
            return "Bool";
          }
          if (ns_name == "MoveFile" && call->Args().size() == 2) {
            return "Bool";
          }
          if (ns_name == "CopyFile" && call->Args().size() == 2) {
            return "Bool";
          }
          if (ns_name == "ListDirectory" && call->Args().size() == 1) {
            return "StringArray";
          }
          if (ns_name == "GetCurrentDirectory" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "ChangeDirectory" && call->Args().size() == 1) {
            return "Bool";
          }
          // Process Control
          if (ns_name == "Sleep" && call->Args().size() == 1) {
            return "Void";
          }
          if (ns_name == "SleepMs" && call->Args().size() == 1) {
            return "Void";
          }
          if (ns_name == "SleepNs" && call->Args().size() == 1) {
            return "Void";
          }
          if (ns_name == "Exit" && call->Args().size() == 1) {
            return "Never";
          }
          if (ns_name == "GetEnvironmentVar" && call->Args().size() == 1) {
            return "String?"; // Returns Nullable<String>
          }
          if (ns_name == "SetEnvironmentVar" && call->Args().size() == 2) {
            return "Bool";
          }
          // Random Number Generation
          if (ns_name == "SeedRandom" && call->Args().size() == 1) {
            return "Void";
          }
          // System Information
          if (ns_name == "GetOsVersion" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "GetArchitecture" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "GetUserName" && call->Args().size() == 0) {
            return "String";
          }
          if (ns_name == "GetHomeDirectory" && call->Args().size() == 0) {
            return "String";
          }
          // Memory and Performance
          if (ns_name == "ForceGarbageCollection" && call->Args().size() == 0) {
            return "Void";
          }
          // FFI
          if (ns_name == "Interop" && call->Args().size() == 4) {
            return "int";
          }
          // I/O functions that return Void
          if (ns_name == "Print" && call->Args().size() == 1) {
            return "Void";
          }
          if (ns_name == "PrintLine" && call->Args().size() == 1) {
            return "Void";
          }
        }
      }
    }

    // Handle method calls on builtin types (e.g., array.Length(), array.GetAt(0))
    if (auto* field_access = dynamic_cast<FieldAccess*>(&call->MutableCallee())) {
      std::string method_name = field_access->Name();
      std::string object_type = GetTypeNameForExpr(&field_access->MutableObject());

      if (!object_type.empty() && kBuiltinTypeNames.contains(object_type)) {
        // Handle array methods
        if (object_type.find("Array") != std::string::npos) {
          if (method_name == "Length" || method_name == "Capacity") {
            return "int";
          }
          if (method_name == "GetAt") {
            return GetElementTypeForArray(object_type);
          }
          if (method_name == "ToString") {
            return "String";
          }
          if (method_name == "GetHash") {
            return "int";
          }
        }
        // Handle String methods
        if (object_type == "String") {
          if (method_name == "Length") {
            return "int";
          }
          if (method_name == "Substring") {
            return "String";
          }
          if (method_name == "Compare") {
            return "int";
          }
          if (method_name == "ToUtf8Bytes") {
            return "ByteArray";
          }
        }
        // Handle wrapper type methods (Int, Float, etc.)
        if (method_name == "ToString") {
          return "String";
        }
        if (method_name == "GetHash") {
          return "int";
        }
      }

      // Handle method calls on user-defined types
      // Check method_return_types_ for the return type of this method
      if (!object_type.empty() && !kBuiltinTypeNames.contains(object_type)) {
        std::string method_key = object_type + "::" + method_name;
        if (const auto it = method_return_types_.find(method_key); it != method_return_types_.end()) {
          return it->second;
        }
      }

      // Handle chained method calls: if object_type is "unknown", it might be a Call expression
      // Try to determine the return type of the nested call to use as object_type
      if (object_type == "unknown" || object_type.empty()) {
        if (auto* nested_call = dynamic_cast<Call*>(&field_access->MutableObject())) {
          // Get the return type of the nested call by examining its structure
          if (auto* nested_field_access = dynamic_cast<FieldAccess*>(&nested_call->MutableCallee())) {
            std::string nested_method_name = nested_field_access->Name();
            std::string nested_object_type = GetTypeNameForExpr(&nested_field_access->MutableObject());
            
            // Check if this is a method call on a user-defined type
            if (!nested_object_type.empty() && !kBuiltinTypeNames.contains(nested_object_type)) {
              std::string nested_method_key = nested_object_type + "::" + nested_method_name;
              if (const auto nested_it = method_return_types_.find(nested_method_key); nested_it != method_return_types_.end()) {
                // Use the return type of the nested call as the object type for this call
                std::string return_type = nested_it->second;
                std::string chained_method_key = return_type + "::" + method_name;
                if (const auto chained_it = method_return_types_.find(chained_method_key); chained_it != method_return_types_.end()) {
                  return chained_it->second;
                }
                // If the chained method doesn't exist, at least return the return type of the nested call
                // This helps with type propagation in chained calls
                return return_type;
              }
            }
          }
        }
      }
    }
  }

  if (const auto* cast = dynamic_cast<CastAs*>(expr)) {
    return TypeToMangledName(cast->Type());
  }

  // For Call expressions, if we couldn't determine the type, return "unknown"
  // to avoid infinite recursion (GetOperandTypeName -> DetermineOperandType -> GetTypeNameForExpr)
  if (dynamic_cast<Call*>(expr) != nullptr) {
    return "unknown";
  }

  return GetOperandTypeName(expr);
}

bool BytecodeVisitor::IsPrimitiveWrapper(const std::string& type_name) {
  return type_name == "Int" || type_name == "Float" || type_name == "Byte" || type_name == "Char" ||
         type_name == "Bool";
}

bool BytecodeVisitor::IsPrimitiveType(const std::string& type_name) const {
  return type_name == "int" || type_name == "float" || type_name == "byte" || type_name == "char" ||
         type_name == "bool";
}

std::string BytecodeVisitor::GetPrimitiveTypeForWrapper(const std::string& wrapper_type) const {
  if (wrapper_type == "Int") {
    return "int";
  }
  if (wrapper_type == "Float") {
    return "float";
  }
  if (wrapper_type == "Byte") {
    return "byte";
  }
  if (wrapper_type == "Char") {
    return "char";
  }
  if (wrapper_type == "Bool") {
    return "bool";
  }
  return wrapper_type;
}

std::string BytecodeVisitor::GetWrapperTypeForPrimitive(const std::string& primitive_type) const {
  if (primitive_type == "int") {
    return "Int";
  }
  if (primitive_type == "float") {
    return "Float";
  }
  if (primitive_type == "byte") {
    return "Byte";
  }
  if (primitive_type == "char") {
    return "Char";
  }
  if (primitive_type == "bool") {
    return "Bool";
  }
  return primitive_type;
}

void BytecodeVisitor::EmitUnwrapIfNeeded(const std::string& type_name) {
  if (IsPrimitiveWrapper(type_name)) {
    EmitCommand("Unwrap");
  }
}

void BytecodeVisitor::EmitWrapIfNeeded(const std::string& expected_type) {
  if (IsPrimitiveWrapper(expected_type)) {
    std::string primitive_type = GetPrimitiveTypeForWrapper(expected_type);
    EmitWrapConstructorCall(expected_type, primitive_type);
  }
}

void BytecodeVisitor::EmitTypeConversionIfNeeded(const std::string& expected_type, const std::string& actual_type) {
  if (IsPrimitiveWrapper(expected_type) && IsPrimitiveType(actual_type)) {
    EmitWrapConstructorCall(expected_type, actual_type);
  } else if (IsPrimitiveType(expected_type) && IsPrimitiveWrapper(actual_type)) {
    EmitCommand("Unwrap");
  }
}

void BytecodeVisitor::EmitWrapConstructorCall(const std::string& wrapper_type, const std::string& primitive_type) {
  std::string constructor_name = "_" + wrapper_type + "_" + primitive_type;
  EmitCommandWithStringWithoutBraces("CallConstructor", constructor_name);
}

int BytecodeVisitor::FindFieldIndex(const std::string& class_name, const std::string& field_name) {
  if (const auto fields_it = class_fields_.find(class_name); fields_it != class_fields_.end()) {
    const auto& fields = fields_it->second;
    for (size_t i = 0; i < fields.size(); ++i) {
      if (fields[i].first == field_name) {
        return static_cast<int>(i);
      }
    }
  }
  return -1;
}

std::string BytecodeVisitor::GetFieldTypeName(const std::string& class_name, const std::string& field_name) {
  if (const auto fields_it = class_fields_.find(class_name); fields_it != class_fields_.end()) {
    for (const auto& fields = fields_it->second; const auto& [fst, snd] : fields) {
      if (fst == field_name) {
        return TypeToMangledName(snd);
      }
    }
  }
  return "";
}

void BytecodeVisitor::EmitBinaryOperatorCommand(const IBinaryOpTag& op, OperandType dominant_type) {
  const auto emit_type_command =
      [this](OperandType type, const std::string& float_cmd, const std::string& byte_cmd, const std::string& int_cmd) {
        if (type == OperandType::kFloat) {
          EmitCommand(float_cmd);
        } else if (type == OperandType::kByte) {
          EmitCommand(byte_cmd);
        } else {
          EmitCommand(int_cmd);
        }
      };

  if (&op == &optags::Add()) {
    emit_type_command(dominant_type, "FloatAdd", "ByteAdd", "IntAdd");
  } else if (&op == &optags::Sub()) {
    emit_type_command(dominant_type, "FloatSubtract", "ByteSubtract", "IntSubtract");
  } else if (&op == &optags::Mul()) {
    emit_type_command(dominant_type, "FloatMultiply", "ByteMultiply", "IntMultiply");
  } else if (&op == &optags::Div()) {
    emit_type_command(dominant_type, "FloatDivide", "ByteDivide", "IntDivide");
  } else if (&op == &optags::Mod()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteModulo");
    } else {
      EmitCommand("IntModulo");
    }
  } else if (&op == &optags::Lt()) {
    emit_type_command(dominant_type, "FloatLessThan", "ByteLessThan", "IntLessThan");
  } else if (&op == &optags::Le()) {
    emit_type_command(dominant_type, "FloatLessEqual", "ByteLessEqual", "IntLessEqual");
  } else if (&op == &optags::Gt()) {
    emit_type_command(dominant_type, "FloatGreaterThan", "ByteGreaterThan", "IntGreaterThan");
  } else if (&op == &optags::Ge()) {
    emit_type_command(dominant_type, "FloatGreaterEqual", "ByteGreaterEqual", "IntGreaterEqual");
  } else if (&op == &optags::Eq()) {
    emit_type_command(dominant_type, "FloatEqual", "ByteEqual", "IntEqual");
  } else if (&op == &optags::Ne()) {
    emit_type_command(dominant_type, "FloatNotEqual", "ByteNotEqual", "IntNotEqual");
  } else if (&op == &optags::And()) {
    EmitCommand("BoolAnd");
  } else if (&op == &optags::Or()) {
    EmitCommand("BoolOr");
  } else if (&op == &optags::Xor()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteXor");
    } else if (dominant_type == OperandType::kInt) {
      EmitCommand("IntXor");
    } else {
      EmitCommand("BoolXor");
    }
  } else if (&op == &optags::BitwiseAnd()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteAnd");
    } else {
      EmitCommand("IntAnd");
    }
  } else if (&op == &optags::BitwiseOr()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteOr");
    } else {
      EmitCommand("IntOr");
    }
  } else if (&op == &optags::LeftShift()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteLeftShift");
    } else {
      EmitCommand("IntLeftShift");
    }
  } else if (&op == &optags::RightShift()) {
    if (dominant_type == OperandType::kByte) {
      EmitCommand("ByteRightShift");
    } else {
      EmitCommand("IntRightShift");
    }
  } else {
    EmitCommand("UnsupportedBinaryOp");
  }
}

std::string BytecodeVisitor::GenerateArrayLengthMethodName(const std::string& array_type) {
  return "_" + array_type + "_Length_<C>";
}

std::string BytecodeVisitor::GenerateArrayGetAtMethodName(const std::string& array_type) {
  GetElementTypeForArray(array_type);
  return "_" + array_type + "_GetAt_<C>_int";
}

std::string BytecodeVisitor::GenerateArraySetAtMethodName(const std::string& array_type) {
  const std::string elem_type = GetElementTypeForArray(array_type);
  return "_" + array_type + "_SetAt_<M>_int_" + elem_type;
}

std::string BytecodeVisitor::GenerateArrayMethodName(const std::string& array_type, const std::string& method_name) {
  std::string base_name = "_" + array_type + "_" + method_name;
  std::string elem_type = GetElementTypeForArray(array_type);

  if (method_name == "Length" || method_name == "GetHash" || method_name == "ToString" || method_name == "Equals" ||
      method_name == "Capacity" || method_name == "GetAt") {
    base_name += "_<C>";
  } else {
    base_name += "_<M>";
  }

  if (method_name == "Add") {
    base_name += "_" + elem_type;
  } else if (method_name == "GetAt" || method_name == "RemoveAt" || method_name == "Reserve") {
    base_name += "_int";
  } else if (method_name == "SetAt" || method_name == "InsertAt") {
    base_name += "_int_" + elem_type;
  } else if (method_name == "Equals") {
    base_name += "_Object";
  }

  return base_name;
}

std::string BytecodeVisitor::GetElementTypeForArray(const std::string& array_type) {
  if (array_type == "IntArray") {
    return "int";
  }
  if (array_type == "FloatArray") {
    return "float";
  }
  if (array_type == "ByteArray") {
    return "byte";
  }
  if (array_type == "BoolArray") {
    return "bool";
  }
  if (array_type == "CharArray") {
    return "char";
  }
  if (array_type == "StringArray") {
    return "String";
  }
  return "Object";
}

bool BytecodeVisitor::IsBuiltinSystemCommand(const std::string& name) const {
  return kBuiltinSystemCommands.contains(name);
}

std::string BytecodeVisitor::ResolveFunctionOverload(const std::string& func_name,
                                                     const std::vector<std::unique_ptr<Expr>>& args) {
  auto overloads_it = function_overloads_.find(func_name);
  if (overloads_it == function_overloads_.end()) {
    return "";
  }

  const auto& overloads = overloads_it->second;
  if (overloads.empty()) {
    return "";
  }

  // If only one overload, return it (no resolution needed)
  if (overloads.size() == 1) {
    return overloads[0].mangled_name;
  }

  // Get argument types
  std::vector<std::string> arg_types;
  arg_types.reserve(args.size());
  for (const auto& arg : args) {
    arg_types.push_back(GetTypeNameForExpr(arg.get()));
  }

  // Find the best matching overload
  int best_match_score = -1;
  size_t best_match_index = 0;

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
      const std::string& expected_type = TypeToMangledName(overload.param_types[j]);
      const std::string& actual_type = arg_types[j];

      if (expected_type == actual_type) {
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
    }
  }

  if (best_match_score >= 0) {
    return overloads[best_match_index].mangled_name;
  }

  return "";
}

bool BytecodeVisitor::TypesCompatible(const std::string& expected_type, const std::string& actual_type) {
  // Exact match
  if (expected_type == actual_type) {
    return true;
  }

  // Primitive to wrapper conversions
  if (IsPrimitiveWrapper(expected_type) && IsPrimitiveType(actual_type)) {
    std::string expected_primitive = GetPrimitiveTypeForWrapper(expected_type);
    return expected_primitive == actual_type;
  }

  // Wrapper to primitive conversions
  if (IsPrimitiveType(expected_type) && IsPrimitiveWrapper(actual_type)) {
    std::string actual_primitive = GetPrimitiveTypeForWrapper(actual_type);
    return expected_type == actual_primitive;
  }

  // Numeric conversions (int <-> float, etc.)
  if ((expected_type == "int" || expected_type == "Int") && (actual_type == "float" || actual_type == "Float")) {
    return true; // float can be converted to int (with truncation)
  }
  if ((expected_type == "float" || expected_type == "Float") && (actual_type == "int" || actual_type == "Int")) {
    return true; // int can be converted to float
  }

  // Object compatibility (any object can be assigned to Object)
  if (expected_type == "Object") {
    return true;
  }

  return false;
}

void BytecodeVisitor::EmitParameterConversions(const std::vector<std::unique_ptr<Expr>>& args,
                                               const std::vector<TypeReference>& expected_types) {
  for (size_t i = args.size(); i > 0; --i) {
    size_t arg_idx = i - 1;
    Expr* arg = args[arg_idx].get();
    arg->Accept(*this);

    if (arg_idx < expected_types.size()) {
      const TypeReference& expected_type = expected_types[arg_idx];
      std::string expected_type_name = TypeToMangledName(expected_type);
      std::string arg_type_name = GetTypeNameForExpr(arg);
      EmitTypeConversionIfNeeded(expected_type_name, arg_type_name);
    }
  }
}

void BytecodeVisitor::EmitArgumentsInReverse(const std::vector<std::unique_ptr<Expr>>& args) {
  for (const auto& arg : std::ranges::reverse_view(args)) {
    arg->Accept(*this);
  }
}

} // namespace ovum::compiler::parser
