#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>
#include "lib/lexer/Lexer.hpp"
#include "test_suites/LexerUnitTestSuite.hpp"


TEST(LexerUnitTestSuite, ExampleFundamentals) {
  const std::string src = R"OVUM(fun ExampleFundamentals(): Void {
  val i: int = 42
  val f: float = 3.14
  val b: byte = 255
  val c: char = 'A'
  val bl: bool = true
  val p: pointer = null
  sys::Print(i.ToString())
  })OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun","ExampleFundamentals","(",")",":","Void","{","\\n",
      "val","i",":","int","=","42","\\n",
      "val","f",":","float","=","3.14","\\n",
      "val","b",":","byte","=","255","\\n",
      "val","c",":","char","=","'A'","\\n",
      "val","bl",":","bool","=","true","\\n",
      "val","p",":","pointer","=","null","\\n",
      "sys","::", "Print","(","i",".","ToString","(",")",")","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",        "PUNCT",        "PUNCT",       "PUNCT",         "IDENT",       "PUNCT",   "NEWLINE",
      "KEYWORD",  "IDENT",        "PUNCT",        "IDENT",       "OPERATOR",      "LITERAL:Int", "NEWLINE", "KEYWORD",
      "IDENT",    "PUNCT",        "IDENT",        "OPERATOR",    "LITERAL:Float", "NEWLINE",     "KEYWORD", "IDENT",
      "PUNCT",    "IDENT",        "OPERATOR",     "LITERAL:Int", "NEWLINE",       "KEYWORD",     "IDENT",   "PUNCT",
      "IDENT",    "OPERATOR",     "LITERAL:Char", "NEWLINE",     "KEYWORD",       "IDENT",       "PUNCT",   "IDENT",
      "OPERATOR", "LITERAL:Bool", "NEWLINE",      "KEYWORD",     "IDENT",         "PUNCT",       "IDENT",   "OPERATOR",
      "KEYWORD",  "NEWLINE",      "IDENT",        "OPERATOR",    "IDENT",         "PUNCT",       "IDENT",   "OPERATOR",
      "IDENT",    "PUNCT",        "PUNCT",        "PUNCT",       "NEWLINE",       "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, ExampleReferences) {
  const std::string src = R"OVUM(fun ExampleReferences(): Void {
val I: Int = 42
val F: Float = 3.14
val B: Byte = 255
val C: Char = 'A'
val Bl: Bool = true
val P: Pointer = null
val count: Int = 0
val pi: Float = 3.14
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun","ExampleReferences","(",")",":","Void","{","\\n",
      "val","I",":","Int","=","42","\\n",
      "val","F",":","Float","=","3.14","\\n",
      "val","B",":","Byte","=","255","\\n",
      "val","C",":","Char","=","'A'","\\n",
      "val","Bl",":","Bool","=","true","\\n",
      "val","P",":","Pointer","=","null","\\n",
      "val","count",":","Int","=","0","\\n",
      "val","pi",":","Float","=","3.14","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",   "PUNCT", "PUNCT", "PUNCT", "IDENT",    "PUNCT",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Int",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Float",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Int",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Char",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Bool",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "KEYWORD",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Int",
      "NEWLINE", "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Float",
      "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, ExampleNullable) {
  const std::string src = R"OVUM(fun ExampleNullable(): Void {
val optInt: Int? = null
val optStr: String? = "Hello"
val optArr: IntArray? = null
val safeInt: Int? = 42
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ExampleNullable", "(", ")",        ":", "Void", "{",         "\\n",
      "val", "optInt",          ":", "Int",      "?", "=",    "null",      "\\n",
      "val", "optStr",          ":", "String",   "?", "=",    "\"Hello\"", "\\n",
      "val", "optArr",          ":", "IntArray", "?", "=",    "null",      "\\n",
      "val", "safeInt",         ":", "Int",      "?", "=",    "42",        "\\n",
      "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",       "PUNCT",    "PUNCT",          "IDENT",   "PUNCT",   "NEWLINE", "KEYWORD",
      "IDENT",    "PUNCT",    "IDENT",       "OPERATOR", "OPERATOR",       "KEYWORD", "NEWLINE", "KEYWORD", "IDENT",
      "PUNCT",    "IDENT",    "OPERATOR",    "OPERATOR", "LITERAL:String", "NEWLINE", "KEYWORD", "IDENT",   "PUNCT",
      "IDENT",    "OPERATOR", "OPERATOR",    "KEYWORD",  "NEWLINE",        "KEYWORD", "IDENT",   "PUNCT",   "IDENT",
      "OPERATOR", "OPERATOR", "LITERAL:Int", "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, ExampleArrays) {
  const std::string src = R"OVUM(fun ExampleArrays(): Void {
val intArr: IntArray = IntArray(5)
val strArr: StringArray = StringArray(3)
intArr[0] := 1
intArr[1] := 2
val emptyList: IntArray = IntArray(0)
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun","ExampleArrays","(",")",":","Void","{","\\n",
      "val","intArr",":","IntArray","=","IntArray","(","5",")","\\n",
      "val","strArr",":","StringArray","=","StringArray","(","3",")","\\n",
      "intArr","[","0","]",":=","1","\\n",
      "intArr","[","1","]",":=","2","\\n",
      "val","emptyList",":","IntArray","=","IntArray","(","0",")","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",     "IDENT",       "PUNCT",   "PUNCT",   "PUNCT",    "IDENT",       "PUNCT",       "NEWLINE",
      "KEYWORD",     "IDENT",       "PUNCT",   "IDENT",   "OPERATOR", "IDENT",       "PUNCT",       "LITERAL:Int",
      "PUNCT",       "NEWLINE",     "KEYWORD", "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",    "IDENT",
      "PUNCT",       "LITERAL:Int", "PUNCT",   "NEWLINE", "IDENT",    "PUNCT",       "LITERAL:Int", "PUNCT",
      "OPERATOR",    "LITERAL:Int", "NEWLINE", "IDENT",   "PUNCT",    "LITERAL:Int", "PUNCT",       "OPERATOR",
      "LITERAL:Int", "NEWLINE",     "KEYWORD", "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",    "IDENT",
      "PUNCT",       "LITERAL:Int", "PUNCT",   "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, ExampleObjects) {
  const std::string src = R"OVUM(interface IShape { fun Area(): float }
class Circle implements IShape {
public val Radius: float
public fun Circle(r: float): Circle { this.Radius = r return this }
public override fun Area(): float { return 3.14 * Radius * Radius }
}
fun ExampleObjects(): Void {
val shape: IShape = Circle(5.0)
val circle: Circle? = Circle(10.0)
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "interface", "IShape", "{",     "fun",    "Area",     "(",          ")",      ":",
      "float",     "}",      "\\n",   "class",  "Circle",   "implements", "IShape", "{",
      "\\n",       "public", "val",   "Radius", ":",        "float",      "\\n",    "public",
      "fun",       "Circle", "(",     "r",      ":",        "float",      ")",      ":",
      "Circle",    "{",      "this",  ".",      "Radius",   "=",          "r",      "return",
      "this",      "}",      "\\n",   "public", "override", "fun",        "Area",   "(",
      ")",         ":",      "float", "{",      "return",   "3.14",       "*",      "Radius",
      "*",         "Radius", "}",     "\\n",    "}",        "\\n",        "fun",    "ExampleObjects",
      "(",         ")",      ":",     "Void",   "{",        "\\n",        "val",    "shape",
      ":",         "IShape", "=",     "Circle", "(",        "5.0",        ")",      "\\n",
      "val",       "circle", ":",     "Circle", "?",        "=",          "Circle", "(",
      "10.0",      ")",      "\\n",   "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",       "IDENT",   "PUNCT",    "KEYWORD",  "IDENT",    "PUNCT",         "PUNCT",    "PUNCT",
      "IDENT",         "PUNCT",   "NEWLINE",  "KEYWORD",  "IDENT",    "KEYWORD",       "IDENT",    "PUNCT",
      "NEWLINE",       "KEYWORD", "KEYWORD",  "IDENT",    "PUNCT",    "IDENT",         "NEWLINE",  "KEYWORD",
      "KEYWORD",       "IDENT",   "PUNCT",    "IDENT",    "PUNCT",    "IDENT",         "PUNCT",    "PUNCT",
      "IDENT",         "PUNCT",   "IDENT",    "OPERATOR", "IDENT",    "OPERATOR",      "IDENT",    "KEYWORD",
      "IDENT",         "PUNCT",   "NEWLINE",  "KEYWORD",  "KEYWORD",  "KEYWORD",       "IDENT",    "PUNCT",
      "PUNCT",         "PUNCT",   "IDENT",    "PUNCT",    "KEYWORD",  "LITERAL:Float", "OPERATOR", "IDENT",
      "OPERATOR",      "IDENT",   "PUNCT",    "NEWLINE",  "PUNCT",    "NEWLINE",       "KEYWORD",  "IDENT",
      "PUNCT",         "PUNCT",   "PUNCT",    "IDENT",    "PUNCT",    "NEWLINE",       "KEYWORD",  "IDENT",
      "PUNCT",         "IDENT",   "OPERATOR", "IDENT",    "PUNCT",    "LITERAL:Float", "PUNCT",    "NEWLINE",
      "KEYWORD",       "IDENT",   "PUNCT",    "IDENT",    "OPERATOR", "OPERATOR",      "IDENT",    "PUNCT",
      "LITERAL:Float", "PUNCT",   "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, ExampleAccess) {
  const std::string src = R"OVUM(class ExampleClass {
private val immutable: int = 10
public var mutable: float = 20.5
}
fun ExampleAccess(): Void {
val obj: ExampleClass = ExampleClass()
obj.mutable = 30.0
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "class","ExampleClass","{","\\n",
      "private","val","immutable",":","int","=","10","\\n",
      "public","var","mutable",":","float","=","20.5","\\n",
      "}","\\n",
      "fun","ExampleAccess","(",")",":","Void","{","\\n",
      "val","obj",":","ExampleClass","=","ExampleClass","(",")","\\n",
      "obj",".","mutable","=","30.0","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",         "NEWLINE",       "KEYWORD", "KEYWORD", "IDENT",   "PUNCT",
      "IDENT",    "OPERATOR", "LITERAL:Int",   "NEWLINE",       "KEYWORD", "KEYWORD", "IDENT",   "PUNCT",
      "IDENT",    "OPERATOR", "LITERAL:Float", "NEWLINE",       "PUNCT",   "NEWLINE", "KEYWORD", "IDENT",
      "PUNCT",    "PUNCT",    "PUNCT",         "IDENT",         "PUNCT",   "NEWLINE", "KEYWORD", "IDENT",
      "PUNCT",    "IDENT",    "OPERATOR",      "IDENT",         "PUNCT",   "PUNCT",   "NEWLINE", "IDENT",
      "OPERATOR", "IDENT",    "OPERATOR",      "LITERAL:Float", "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, SimpleIf) {
  const std::string src = R"OVUM(fun SimpleIf(x: int): Void {
if (x > 0) { sys::Print("Positive") }
else if (x < 0) { sys::Print("Negative") }
else { sys::Print("Zero") }
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun","SimpleIf","(","x",":","int",")",":","Void","{","\\n",
      "if","(","x",">","0",")","{","sys","::", "Print","(","\"Positive\"",")","}","\\n",
      "else","if","(","x","<","0",")","{","sys","::", "Print","(","\"Negative\"",")","}","\\n",
      "else","{","sys","::", "Print","(","\"Zero\"",")","}","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",        "IDENT",   "PUNCT",   "IDENT",    "PUNCT", "IDENT",    "PUNCT",          "PUNCT",
      "IDENT",          "PUNCT",   "NEWLINE", "KEYWORD",  "PUNCT", "IDENT",    "OPERATOR",       "LITERAL:Int",
      "PUNCT",          "PUNCT",   "IDENT",   "OPERATOR", "IDENT", "PUNCT",    "LITERAL:String", "PUNCT",
      "PUNCT",          "NEWLINE", "KEYWORD", "KEYWORD",  "PUNCT", "IDENT",    "OPERATOR",       "LITERAL:Int",
      "PUNCT",          "PUNCT",   "IDENT",   "OPERATOR", "IDENT", "PUNCT",    "LITERAL:String", "PUNCT",
      "PUNCT",          "NEWLINE", "KEYWORD", "PUNCT",    "IDENT", "OPERATOR", "IDENT",          "PUNCT",
      "LITERAL:String", "PUNCT",   "PUNCT",   "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, BooleanConditions) {
  const std::string src = R"OVUM(fun BooleanConditions(a: bool, b: bool): Void {
if (a && b) { sys::Print("Both true") }
else if (a || b) { sys::Print("One true") }
else if (!a ^ b) { sys::Print("Exactly one false") }
else { sys::Print("Both false") }
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun","BooleanConditions","(","a",":","bool",",","b",":","bool",")",":","Void","{","\\n",
      "if","(","a","&&","b",")","{","sys","::", "Print","(","\"Both true\"",")","}","\\n",
      "else","if","(","a","||","b",")","{","sys","::", "Print","(","\"One true\"",")","}","\\n",
      "else","if","(","!","a","^","b",")","{","sys","::", "Print","(","\"Exactly one false\"",")","}","\\n",
      "else","{","sys","::", "Print","(","\"Both false\"",")","}","\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",          "IDENT",          "PUNCT", "IDENT",          "PUNCT",   "IDENT",
      "PUNCT",    "IDENT",    "PUNCT",          "PUNCT",          "IDENT", "PUNCT",          "NEWLINE", "KEYWORD",
      "PUNCT",    "IDENT",    "OPERATOR",       "IDENT",          "PUNCT", "PUNCT",          "IDENT",   "OPERATOR",
      "IDENT",    "PUNCT",    "LITERAL:String", "PUNCT",          "PUNCT", "NEWLINE",        "KEYWORD", "KEYWORD",
      "PUNCT",    "IDENT",    "OPERATOR",       "IDENT",          "PUNCT", "PUNCT",          "IDENT",   "OPERATOR",
      "IDENT",    "PUNCT",    "LITERAL:String", "PUNCT",          "PUNCT", "NEWLINE",        "KEYWORD", "KEYWORD",
      "PUNCT",    "OPERATOR", "IDENT",          "OPERATOR",       "IDENT", "PUNCT",          "PUNCT",   "IDENT",
      "OPERATOR", "IDENT",    "PUNCT",          "LITERAL:String", "PUNCT", "PUNCT",          "NEWLINE", "KEYWORD",
      "PUNCT",    "IDENT",    "OPERATOR",       "IDENT",          "PUNCT", "LITERAL:String", "PUNCT",   "PUNCT",
      "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, TypeConditions) {
  const std::string src = R"OVUM(fun TypeConditions(obj: Object): Void {
if (obj is String) { val str: String? = obj as String if (str != null) { sys::Print(str.Length().ToString()) } }
else if (obj is Int) { val num: Int = obj as Int sys::Print(num.ToString()) }
else { sys::Print("Unknown type") }
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "TypeConditions", "(", "obj", ":", "Object", ")", ":", "Void", "{",
      "\\n",
      "if", "(", "obj", "is", "String", ")", "{", "val", "str", ":", "String", "?", "=", "obj", "as", "String", "if",
      "(", "str", "!=", "null", ")", "{", "sys", "::", "Print", "(", "str", ".", "Length", "(", ")", ".", "ToString",
      "(", ")", ")", "}", "}",
      "\\n",
      "else", "if", "(", "obj", "is", "Int", ")", "{", "val", "num", ":", "Int", "=", "obj", "as", "Int", "sys", "::",
      "Print", "(", "num", ".", "ToString", "(", ")", ")", "}",
      "\\n",
      "else", "{", "sys", "::", "Print", "(", "\"Unknown type\"", ")", "}",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",   "IDENT",    "PUNCT",          "IDENT",    "PUNCT",   "PUNCT",    "IDENT",
      "PUNCT",    "NEWLINE",  "KEYWORD", "PUNCT",    "IDENT",          "KEYWORD",  "IDENT",   "PUNCT",    "PUNCT",
      "KEYWORD",  "IDENT",    "PUNCT",   "IDENT",    "OPERATOR",       "OPERATOR", "IDENT",   "KEYWORD",  "IDENT",
      "KEYWORD",  "PUNCT",    "IDENT",   "OPERATOR", "KEYWORD",        "PUNCT",    "PUNCT",   "IDENT",    "OPERATOR",
      "IDENT",    "PUNCT",    "IDENT",   "OPERATOR", "IDENT",          "PUNCT",    "PUNCT",   "OPERATOR", "IDENT",
      "PUNCT",    "PUNCT",    "PUNCT",   "PUNCT",    "PUNCT",          "NEWLINE",  "KEYWORD", "KEYWORD",  "PUNCT",
      "IDENT",    "KEYWORD",  "IDENT",   "PUNCT",    "PUNCT",          "KEYWORD",  "IDENT",   "PUNCT",    "IDENT",
      "OPERATOR", "IDENT",    "KEYWORD", "IDENT",    "IDENT",          "OPERATOR", "IDENT",   "PUNCT",    "IDENT",
      "OPERATOR", "IDENT",    "PUNCT",   "PUNCT",    "PUNCT",          "PUNCT",    "NEWLINE", "KEYWORD",  "PUNCT",
      "IDENT",    "OPERATOR", "IDENT",   "PUNCT",    "LITERAL:String", "PUNCT",    "PUNCT",   "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ElvisInIf) {
  const std::string src = R"OVUM(fun ElvisInIf(opt: Int?): int {
val value: int = opt ?: 0
if (value > 10) { return value * 2 }
else { return value }
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun",    "ElvisInIf", "(", "opt", ":",  "Int", "?",    ")",  ":",      "int",   "{", "\\n", "val", "value",
      ":",      "int",       "=", "opt", "?:", "0",   "\\n",  "if", "(",      "value", ">", "10",  ")",   "{",
      "return", "value",     "*", "2",   "}",  "\\n", "else", "{",  "return", "value", "}", "\\n", "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",       "PUNCT",   "IDENT",    "PUNCT",       "IDENT", "OPERATOR", "PUNCT",       "PUNCT",
      "IDENT",    "PUNCT",       "NEWLINE", "KEYWORD",  "IDENT",       "PUNCT", "IDENT",    "OPERATOR",    "IDENT",
      "OPERATOR", "LITERAL:Int", "NEWLINE", "KEYWORD",  "PUNCT",       "IDENT", "OPERATOR", "LITERAL:Int", "PUNCT",
      "PUNCT",    "KEYWORD",     "IDENT",   "OPERATOR", "LITERAL:Int", "PUNCT", "NEWLINE",  "KEYWORD",     "PUNCT",
      "KEYWORD",  "IDENT",       "PUNCT",   "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
TEST(LexerUnitTestSuite, WhenLike) {
  const std::string src = R"OVUM(fun WhenLike(x: int): String {
if (x == 1) return "One"
else if (x == 2) return "Two"
else if (x > 10) return "Big"
else return "Other"
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "WhenLike", "(",       "x",      ":",       "int",    ")",         ":",   "String", "{",  "\\n", "if",
      "(",   "x",        "==",      "1",      ")",       "return", "\"One\"",   "\\n", "else",   "if", "(",   "x",
      "==",  "2",        ")",       "return", "\"Two\"", "\\n",    "else",      "if",  "(",      "x",  ">",   "10",
      ")",   "return",   "\"Big\"", "\\n",    "else",    "return", "\"Other\"", "\\n", "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",        "IDENT",       "PUNCT",   "IDENT",          "PUNCT",          "IDENT",   "PUNCT",
      "PUNCT",          "IDENT",       "PUNCT",   "NEWLINE",        "KEYWORD",        "PUNCT",   "IDENT",
      "OPERATOR",       "LITERAL:Int", "PUNCT",   "KEYWORD",        "LITERAL:String", "NEWLINE", "KEYWORD",
      "KEYWORD",        "PUNCT",       "IDENT",   "OPERATOR",       "LITERAL:Int",    "PUNCT",   "KEYWORD",
      "LITERAL:String", "NEWLINE",     "KEYWORD", "KEYWORD",        "PUNCT",          "IDENT",   "OPERATOR",
      "LITERAL:Int",    "PUNCT",       "KEYWORD", "LITERAL:String", "NEWLINE",        "KEYWORD", "KEYWORD",
      "LITERAL:String", "NEWLINE",     "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, WhileExample) {
  const std::string src = R"OVUM(fun WhileExample(n: int): int {
var counter: int = 0
while (counter < n) {
  counter = counter + 1
  if (counter % 2 == 0) continue
  sys::Print(counter.ToString())
}
return counter
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "WhileExample", "(", "n", ":", "int", ")", ":", "int", "{",
      "\\n",
      "var", "counter", ":", "int", "=", "0",
      "\\n",
      "while", "(", "counter", "<", "n", ")", "{",
      "\\n",
      "counter", "=", "counter", "+", "1",
      "\\n",
      "if", "(", "counter", "%", "2", "==", "0", ")", "continue",
      "\\n",
      "sys", "::", "Print", "(", "counter", ".", "ToString", "(", ")", ")",
      "\\n",
      "}",
      "\\n",
      "return", "counter",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",     "IDENT",   "PUNCT",   "IDENT",    "PUNCT",       "IDENT",    "PUNCT",       "PUNCT",
      "IDENT",       "PUNCT",   "NEWLINE", "KEYWORD",  "IDENT",       "PUNCT",    "IDENT",       "OPERATOR",
      "LITERAL:Int", "NEWLINE", "KEYWORD", "PUNCT",    "IDENT",       "OPERATOR", "IDENT",       "PUNCT",
      "PUNCT",       "NEWLINE", "IDENT",   "OPERATOR", "IDENT",       "OPERATOR", "LITERAL:Int", "NEWLINE",
      "KEYWORD",     "PUNCT",   "IDENT",   "OPERATOR", "LITERAL:Int", "OPERATOR", "LITERAL:Int", "PUNCT",
      "KEYWORD",     "NEWLINE", "IDENT",   "OPERATOR", "IDENT",       "PUNCT",    "IDENT",       "OPERATOR",
      "IDENT",       "PUNCT",   "PUNCT",   "PUNCT",    "NEWLINE",     "PUNCT",    "NEWLINE",     "KEYWORD",
      "IDENT",       "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ForExample) {
  const std::string src = R"OVUM(fun ForExample(arr: IntArray): int {
var sum: int = 0
for (num in arr) { if (num < 0) break sum = sum + num }
return sum
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ForExample", "(", "arr", ":", "IntArray", ")",      ":",   "int", "{",     "\\n",
      "var", "sum",        ":", "int", "=", "0",        "\\n",    "for", "(",   "num",   "in",
      "arr", ")",          "{", "if",  "(", "num",      "<",      "0",   ")",   "break", "sum",
      "=",   "sum",        "+", "num", "}", "\\n",      "return", "sum", "\\n", "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",    "PUNCT",       "IDENT",   "PUNCT",   "IDENT",   "PUNCT",    "PUNCT",       "IDENT",
      "PUNCT",   "NEWLINE",  "KEYWORD",     "IDENT",   "PUNCT",   "IDENT",   "OPERATOR", "LITERAL:Int", "NEWLINE",
      "KEYWORD", "PUNCT",    "IDENT",       "KEYWORD", "IDENT",   "PUNCT",   "PUNCT",    "KEYWORD",     "PUNCT",
      "IDENT",   "OPERATOR", "LITERAL:Int", "PUNCT",   "KEYWORD", "IDENT",   "OPERATOR", "IDENT",       "OPERATOR",
      "IDENT",   "PUNCT",    "NEWLINE",     "KEYWORD", "IDENT",   "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, NestedLoops) {
  const std::string src = R"OVUM(fun NestedLoops(matrix: IntArrayArray): Void {
for (row in matrix) {
var i: int = 0
while (i < row.Length()) {
if (row[i] == 0) { continue }
sys::Print(row[i].ToString())
i = i + 1
if (i > 5) break }
}
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "NestedLoops", "(", "matrix", ":", "IntArrayArray", ")", ":", "Void", "{",
      "\\n",
      "for", "(", "row", "in", "matrix", ")", "{",
      "\\n",
      "var", "i", ":", "int", "=", "0",
      "\\n",
      "while", "(", "i", "<", "row", ".", "Length", "(", ")", ")", "{",
      "\\n",
      "if", "(", "row", "[", "i", "]", "==", "0", ")", "{", "continue", "}",
      "\\n",
      "sys", "::", "Print", "(", "row", "[", "i", "]", ".", "ToString", "(", ")", ")",
      "\\n",
      "i", "=", "i", "+", "1",
      "\\n",
      "if", "(", "i", ">", "5", ")", "break",
      "}",
      "\\n",
      "}",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",     "IDENT",   "PUNCT",    "IDENT",       "PUNCT",    "IDENT",       "PUNCT",   "PUNCT",
      "IDENT",       "PUNCT",   "NEWLINE",  "KEYWORD",     "PUNCT",    "IDENT",       "KEYWORD", "IDENT",
      "PUNCT",       "PUNCT",   "NEWLINE",  "KEYWORD",     "IDENT",    "PUNCT",       "IDENT",   "OPERATOR",
      "LITERAL:Int", "NEWLINE", "KEYWORD",  "PUNCT",       "IDENT",    "OPERATOR",    "IDENT",   "OPERATOR",
      "IDENT",       "PUNCT",   "PUNCT",    "PUNCT",       "PUNCT",    "NEWLINE",     "KEYWORD", "PUNCT",
      "IDENT",       "PUNCT",   "IDENT",    "PUNCT",       "OPERATOR", "LITERAL:Int", "PUNCT",   "PUNCT",
      "KEYWORD",     "PUNCT",   "NEWLINE",  "IDENT",       "OPERATOR", "IDENT",       "PUNCT",   "IDENT",
      "PUNCT",       "IDENT",   "PUNCT",    "OPERATOR",    "IDENT",    "PUNCT",       "PUNCT",   "PUNCT",
      "NEWLINE",     "IDENT",   "OPERATOR", "IDENT",       "OPERATOR", "LITERAL:Int", "NEWLINE", "KEYWORD",
      "PUNCT",       "IDENT",   "OPERATOR", "LITERAL:Int", "PUNCT",    "KEYWORD",     "PUNCT",   "NEWLINE",
      "PUNCT",       "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ForWithIndex) {
  const std::string src = R"OVUM(fun ForWithIndex(arr: IntArray): Void {
var i: int = 0
while (i < arr.Length()) {
sys::Print((i.ToString() + ": " + arr[i].ToString()))
i = i + 1 }})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ForWithIndex", "(", "arr", ":", "IntArray", ")", ":", "Void", "{",
      "\\n",
      "var", "i", ":", "int", "=", "0",
      "\\n",
      "while", "(", "i", "<", "arr", ".", "Length", "(", ")", ")", "{",
      "\\n",
      "sys", "::", "Print", "(", "(", "i", ".", "ToString", "(", ")", "+", "\": \"", "+", "arr", "[", "i", "]", ".",
      "ToString", "(", ")", ")", ")",
      "\\n",
      "i", "=", "i", "+", "1", "}", "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",     "IDENT",          "PUNCT",       "IDENT",   "PUNCT",    "IDENT",    "PUNCT", "PUNCT",
      "IDENT",       "PUNCT",          "NEWLINE",     "KEYWORD", "IDENT",    "PUNCT",    "IDENT", "OPERATOR",
      "LITERAL:Int", "NEWLINE",        "KEYWORD",     "PUNCT",   "IDENT",    "OPERATOR", "IDENT", "OPERATOR",
      "IDENT",       "PUNCT",          "PUNCT",       "PUNCT",   "PUNCT",    "NEWLINE",  "IDENT", "OPERATOR",
      "IDENT",       "PUNCT",          "PUNCT",       "IDENT",   "OPERATOR", "IDENT",    "PUNCT", "PUNCT",
      "OPERATOR",    "LITERAL:String", "OPERATOR",    "IDENT",   "PUNCT",    "IDENT",    "PUNCT", "OPERATOR",
      "IDENT",       "PUNCT",          "PUNCT",       "PUNCT",   "PUNCT",    "NEWLINE",  "IDENT", "OPERATOR",
      "IDENT",       "OPERATOR",       "LITERAL:Int", "PUNCT",   "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ListCreation) {
  const std::string src = R"OVUM(fun ListCreation(): Void {
val list: IntArray = IntArray(4)
list[0] := 10
list[1] := 20
list[2] := 30
list[3] := 40
val fromLiteral: IntArray = IntArray(2)
fromLiteral[0] := 1
fromLiteral[1] := 2
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ListCreation", "(", ")", ":", "Void", "{",
      "\\n",
      "val", "list", ":", "IntArray", "=", "IntArray", "(", "4", ")",
      "\\n",
      "list", "[", "0", "]", ":=", "10",
      "\\n",
      "list", "[", "1", "]", ":=", "20",
      "\\n",
      "list", "[", "2", "]", ":=", "30",
      "\\n",
      "list", "[", "3", "]", ":=", "40",
      "\\n",
      "val", "fromLiteral", ":", "IntArray", "=", "IntArray", "(", "2", ")",
      "\\n",
      "fromLiteral", "[", "0", "]", ":=", "1",
      "\\n",
      "fromLiteral", "[", "1", "]", ":=", "2",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",       "PUNCT",       "PUNCT",   "PUNCT",       "IDENT",       "PUNCT",
      "NEWLINE", "KEYWORD",     "IDENT",       "PUNCT",   "IDENT",       "OPERATOR",    "IDENT",
      "PUNCT",   "LITERAL:Int", "PUNCT",       "NEWLINE", "IDENT",       "PUNCT",       "LITERAL:Int",
      "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE", "IDENT",       "PUNCT",       "LITERAL:Int",
      "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE", "IDENT",       "PUNCT",       "LITERAL:Int",
      "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE", "IDENT",       "PUNCT",       "LITERAL:Int",
      "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE", "KEYWORD",     "IDENT",       "PUNCT",
      "IDENT",   "OPERATOR",    "IDENT",       "PUNCT",   "LITERAL:Int", "PUNCT",       "NEWLINE",
      "IDENT",   "PUNCT",       "LITERAL:Int", "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE",
      "IDENT",   "PUNCT",       "LITERAL:Int", "PUNCT",   "OPERATOR",    "LITERAL:Int", "NEWLINE",
      "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ListIteration) {
  const std::string src = R"OVUM(fun ListIteration(list: IntArray): int {
var total: int = 0
var idx: int = 0
while (idx < list.Length()) { total = total + list[idx]
if (list[idx] % 2 == 0) { list[idx] := list[idx] * 2 } idx = idx + 1 }
return total
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ListIteration", "(", "list", ":", "IntArray", ")", ":", "int", "{",
      "\\n",
      "var", "total", ":", "int", "=", "0",
      "\\n",
      "var", "idx", ":", "int", "=", "0",
      "\\n",
      "while", "(", "idx", "<", "list", ".", "Length", "(", ")", ")", "{", "total", "=", "total", "+", "list", "[",
      "idx", "]", "\\n",
      "if", "(", "list", "[", "idx", "]", "%", "2", "==", "0", ")", "{", "list", "[", "idx", "]", ":=",
      "list", "[", "idx", "]", "*", "2", "}", "idx", "=", "idx", "+", "1", "}",
      "\\n",
      "return", "total",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT", "PUNCT", "IDENT", "PUNCT", "IDENT", "PUNCT", "PUNCT", "IDENT", "PUNCT",
      "NEWLINE",
      "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Int",
      "NEWLINE",
      "KEYWORD", "IDENT", "PUNCT", "IDENT", "OPERATOR", "LITERAL:Int",
      "NEWLINE",
      "KEYWORD", "PUNCT", "IDENT", "OPERATOR", "IDENT", "OPERATOR", "IDENT", "PUNCT", "PUNCT", "PUNCT", "PUNCT", "IDENT", "OPERATOR", "IDENT", "OPERATOR", "IDENT", "PUNCT", "IDENT", "PUNCT",
      "NEWLINE",
      "KEYWORD", "PUNCT", "IDENT", "PUNCT", "IDENT", "PUNCT", "OPERATOR", "LITERAL:Int", "OPERATOR", "LITERAL:Int", "PUNCT", "PUNCT", "IDENT", "PUNCT", "IDENT", "PUNCT", "OPERATOR", "IDENT", "PUNCT", "IDENT", "PUNCT", "OPERATOR", "LITERAL:Int", "PUNCT", "IDENT", "OPERATOR", "IDENT", "OPERATOR", "LITERAL:Int", "PUNCT",
      "NEWLINE",
      "KEYWORD", "IDENT",
      "NEWLINE",
      "PUNCT"
  };
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ListOps) {
  const std::string src = R"OVUM(fun ListOps(original: IntArray): IntArray {
val newList: IntArray = IntArray(original.Length() + 1)
var i: int = 0
while (i < original.Length()) { newList[i] := original[i] i = i + 1 }
newList[original.Length()] := 100
return newList
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ListOps", "(", "original", ":",   "IntArray", ")",        ":",        "IntArray", "{",      "\\n",
      "val", "newList", ":", "IntArray", "=",   "IntArray", "(",        "original", ".",        "Length", "(",
      ")",   "+",       "1", ")",        "\\n", "var",      "i",        ":",        "int",      "=",      "0",
      "\\n", "while",   "(", "i",        "<",   "original", ".",        "Length",   "(",        ")",      ")",
      "{",   "newList", "[", "i",        "]",   ":=",       "original", "[",        "i",        "]",      "i",
      "=",   "i",       "+", "1",        "}",   "\\n",      "newList",  "[",        "original", ".",      "Length",
      "(",   ")",       "]", ":=",       "100", "\\n",      "return",   "newList",  "\\n",      "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",     "IDENT",    "PUNCT",       "IDENT",    "PUNCT",   "IDENT",    "PUNCT",       "PUNCT",
      "IDENT",       "PUNCT",    "NEWLINE",     "KEYWORD",  "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",
      "IDENT",       "PUNCT",    "IDENT",       "OPERATOR", "IDENT",   "PUNCT",    "PUNCT",       "OPERATOR",
      "LITERAL:Int", "PUNCT",    "NEWLINE",     "KEYWORD",  "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",
      "LITERAL:Int", "NEWLINE",  "KEYWORD",     "PUNCT",    "IDENT",   "OPERATOR", "IDENT",       "OPERATOR",
      "IDENT",       "PUNCT",    "PUNCT",       "PUNCT",    "PUNCT",   "IDENT",    "PUNCT",       "IDENT",
      "PUNCT",       "OPERATOR", "IDENT",       "PUNCT",    "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",
      "IDENT",       "OPERATOR", "LITERAL:Int", "PUNCT",    "NEWLINE", "IDENT",    "PUNCT",       "IDENT",
      "OPERATOR",    "IDENT",    "PUNCT",       "PUNCT",    "PUNCT",   "OPERATOR", "LITERAL:Int", "NEWLINE",
      "KEYWORD",     "IDENT",    "NEWLINE",     "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, FindInList) {
  const std::string src = R"OVUM(fun FindInList(list: IntArray, target: int): Int? {
for (num in list) { if (num == target) { return Int(num) } }
return null
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "FindInList", "(", "list", ":", "IntArray", ",", "target", ":", "int", ")", ":", "Int", "?", "{",
      "\\n",
      "for", "(", "num", "in", "list", ")", "{", "if", "(", "num", "==", "target", ")", "{", "return", "Int",
      "(", "num", ")", "}", "}",
      "\\n",
      "return", "null",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",   "PUNCT",   "IDENT",   "PUNCT",    "IDENT",   "PUNCT",   "IDENT",   "PUNCT",
      "IDENT",   "PUNCT",   "PUNCT",   "IDENT",   "OPERATOR", "PUNCT",   "NEWLINE", "KEYWORD", "PUNCT",
      "IDENT",   "KEYWORD", "IDENT",   "PUNCT",   "PUNCT",    "KEYWORD", "PUNCT",   "IDENT",   "OPERATOR",
      "IDENT",   "PUNCT",   "PUNCT",   "KEYWORD", "IDENT",    "PUNCT",   "IDENT",   "PUNCT",   "PUNCT",
      "PUNCT",   "NEWLINE", "KEYWORD", "KEYWORD", "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, SafeCall) {
  const std::string src = R"OVUM(fun SafeCall(obj: String?): Void {
val length: int = obj?.Length() ?: 0
sys::Print(length.ToString())
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun",    "SafeCall", "(",   "obj",    ":",   "String",   "?",      ")", ":", "Void", "{", "\\n", "val",
      "length", ":",        "int", "=",      "obj", "?.",       "Length", "(", ")", "?:",   "0", "\\n", "sys",
      "::",     "Print",    "(",   "length", ".",   "ToString", "(",      ")", ")", "\\n",  "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",   "PUNCT",    "IDENT",       "PUNCT",   "IDENT",   "OPERATOR", "PUNCT", "PUNCT",    "IDENT",
      "PUNCT",    "NEWLINE", "KEYWORD",  "IDENT",       "PUNCT",   "IDENT",   "OPERATOR", "IDENT", "OPERATOR", "IDENT",
      "PUNCT",    "PUNCT",   "OPERATOR", "LITERAL:Int", "NEWLINE", "IDENT",   "OPERATOR", "IDENT", "PUNCT",    "IDENT",
      "OPERATOR", "IDENT",   "PUNCT",    "PUNCT",       "PUNCT",   "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ElvisExamples) {
  const std::string src = R"OVUM(fun ElvisExamples(optInt: Int?): int {
val default: int = optInt ?: 42
val str: String = optInt?.ToString() ?: "Unknown"
return default
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "ElvisExamples", "(", "optInt", ":", "Int", "?", ")", ":", "int", "{",
      "\\n",
      "val", "default", ":", "int", "=", "optInt", "?:", "42",
      "\\n",
      "val", "str", ":", "String", "=", "optInt", "?.", "ToString", "(", ")", "?:", "\"Unknown\"",
      "\\n",
      "return", "default",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",        "IDENT",    "PUNCT",    "IDENT",       "PUNCT",   "IDENT",   "OPERATOR", "PUNCT",
      "PUNCT",          "IDENT",    "PUNCT",    "NEWLINE",     "KEYWORD", "IDENT",   "PUNCT",    "IDENT",
      "OPERATOR",       "IDENT",    "OPERATOR", "LITERAL:Int", "NEWLINE", "KEYWORD", "IDENT",    "PUNCT",
      "IDENT",          "OPERATOR", "IDENT",    "OPERATOR",    "IDENT",   "PUNCT",   "PUNCT",    "OPERATOR",
      "LITERAL:String", "NEWLINE",  "KEYWORD",  "IDENT",       "NEWLINE", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, NullChecks) {
  const std::string src = R"OVUM(fun NullChecks(opt: Object?): Void {
if (opt != null) { sys::Print("Not null") val nonNull: Object = opt }
val safe: Object? = opt
if (safe == null) { sys::Print("Is null") }
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun",    "NullChecks", "(",   "opt",   ":",   "Object",       "?",   ")",           ":",
      "Void",   "{",          "\\n", "if",    "(",   "opt",          "!=",  "null",        ")",
      "{",      "sys",        "::",  "Print", "(",   "\"Not null\"", ")",   "val",         "nonNull",
      ":",      "Object",     "=",   "opt",   "}",   "\\n",          "val", "safe",        ":",
      "Object", "?",          "=",   "opt",   "\\n", "if",           "(",   "safe",        "==",
      "null",   ")",          "{",   "sys",   "::",  "Print",        "(",   "\"Is null\"", ")",
      "}",      "\\n",        "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",    "PUNCT", "IDENT",   "PUNCT",          "IDENT",    "OPERATOR", "PUNCT",
      "PUNCT",   "IDENT",    "PUNCT", "NEWLINE", "KEYWORD",        "PUNCT",    "IDENT",    "OPERATOR",
      "KEYWORD", "PUNCT",    "PUNCT", "IDENT",   "OPERATOR",       "IDENT",    "PUNCT",    "LITERAL:String",
      "PUNCT",   "KEYWORD",  "IDENT", "PUNCT",   "IDENT",          "OPERATOR", "IDENT",    "PUNCT",
      "NEWLINE", "KEYWORD",  "IDENT", "PUNCT",   "IDENT",          "OPERATOR", "OPERATOR", "IDENT",
      "NEWLINE", "KEYWORD",  "PUNCT", "IDENT",   "OPERATOR",       "KEYWORD",  "PUNCT",    "PUNCT",
      "IDENT",   "OPERATOR", "IDENT", "PUNCT",   "LITERAL:String", "PUNCT",    "PUNCT",    "NEWLINE",
      "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, CastNullable) {
  const std::string src = R"OVUM(fun CastNullable(obj: Object?): Void {
if (obj is Int?) {
val casted: Int? = obj as Int?
val value: int = casted ?: 0
sys::Print(value.ToString()) }
val risky: Int = obj as Int
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "CastNullable", "(", "obj", ":", "Object", "?", ")", ":", "Void", "{",
      "\\n",
      "if", "(", "obj", "is", "Int", "?", ")", "{",
      "\\n",
      "val", "casted", ":", "Int", "?", "=", "obj", "as", "Int", "?",
      "\\n",
      "val", "value", ":", "int", "=", "casted", "?:", "0",
      "\\n",
      "sys", "::", "Print", "(", "value", ".", "ToString", "(", ")", ")", "}",
      "\\n",
      "val", "risky", ":", "Int", "=", "obj", "as", "Int",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",    "IDENT",       "PUNCT",   "IDENT",   "OPERATOR", "PUNCT",    "PUNCT",
      "IDENT",    "PUNCT",    "NEWLINE",  "KEYWORD",     "PUNCT",   "IDENT",   "KEYWORD",  "IDENT",    "OPERATOR",
      "PUNCT",    "PUNCT",    "NEWLINE",  "KEYWORD",     "IDENT",   "PUNCT",   "IDENT",    "OPERATOR", "OPERATOR",
      "IDENT",    "KEYWORD",  "IDENT",    "OPERATOR",    "NEWLINE", "KEYWORD", "IDENT",    "PUNCT",    "IDENT",
      "OPERATOR", "IDENT",    "OPERATOR", "LITERAL:Int", "NEWLINE", "IDENT",   "OPERATOR", "IDENT",    "PUNCT",
      "IDENT",    "OPERATOR", "IDENT",    "PUNCT",       "PUNCT",   "PUNCT",   "PUNCT",    "NEWLINE",  "KEYWORD",
      "IDENT",    "PUNCT",    "IDENT",    "OPERATOR",    "IDENT",   "KEYWORD", "IDENT",    "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, CopyNullable) {
  const std::string src = R"OVUM(fun CopyNullable(src: IntArray?): IntArray? {
if (src == null) return null
val copy: IntArray? = IntArray(src.Length())
var i: int = 0
while (i < src.Length()) { copy[i] := src[i] i = i + 1 }
return copy
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "CopyNullable", "(", "src", ":", "IntArray", "?", ")", ":", "IntArray", "?", "{",
      "\\n",
      "if", "(", "src", "==", "null", ")", "return", "null",
      "\\n",
      "val", "copy", ":", "IntArray", "?", "=", "IntArray", "(", "src", ".", "Length", "(", ")", ")",
      "\\n",
      "var", "i", ":", "int", "=", "0",
      "\\n",
      "while", "(", "i", "<", "src", ".", "Length", "(", ")", ")", "{", "copy", "[", "i", "]", ":=", "src",
      "[", "i", "]", "i", "=", "i", "+", "1", "}",
      "\\n",
      "return", "copy",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",    "PUNCT",    "IDENT",   "PUNCT",    "IDENT",       "OPERATOR",    "PUNCT",    "PUNCT",
      "IDENT",    "OPERATOR", "PUNCT",    "NEWLINE", "KEYWORD",  "PUNCT",       "IDENT",       "OPERATOR", "KEYWORD",
      "PUNCT",    "KEYWORD",  "KEYWORD",  "NEWLINE", "KEYWORD",  "IDENT",       "PUNCT",       "IDENT",    "OPERATOR",
      "OPERATOR", "IDENT",    "PUNCT",    "IDENT",   "OPERATOR", "IDENT",       "PUNCT",       "PUNCT",    "PUNCT",
      "NEWLINE",  "KEYWORD",  "IDENT",    "PUNCT",   "IDENT",    "OPERATOR",    "LITERAL:Int", "NEWLINE",  "KEYWORD",
      "PUNCT",    "IDENT",    "OPERATOR", "IDENT",   "OPERATOR", "IDENT",       "PUNCT",       "PUNCT",    "PUNCT",
      "PUNCT",    "IDENT",    "PUNCT",    "IDENT",   "PUNCT",    "OPERATOR",    "IDENT",       "PUNCT",    "IDENT",
      "PUNCT",    "IDENT",    "OPERATOR", "IDENT",   "OPERATOR", "LITERAL:Int", "PUNCT",       "NEWLINE",  "KEYWORD",
      "IDENT",    "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, ChainSafe) {
  const std::string src = R"OVUM(class Nested {
public val Inner: String?
public fun Nested(s: String?): Nested { this.Inner = s return this } }
fun ChainSafe(nested: Nested?): Void {
val len: int = nested?.Inner?.Length() ?: 0
sys::Print(len.ToString())
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "class", "Nested", "{",
      "\\n",
      "public", "val", "Inner", ":", "String", "?",
      "\\n",
      "public", "fun", "Nested", "(", "s", ":", "String", "?", ")", ":", "Nested", "{", "this", ".",
      "Inner", "=", "s", "return", "this", "}","}",
      "\\n",
      "fun", "ChainSafe", "(", "nested", ":", "Nested", "?", ")", ":", "Void", "{",
      "\\n",
      "val", "len", ":", "int", "=", "nested", "?.", "Inner", "?.", "Length", "(", ")", "?:", "0",
      "\\n",
      "sys", "::", "Print", "(", "len", ".", "ToString", "(", ")", ")",
      "\\n",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD",  "IDENT",   "PUNCT",    "NEWLINE",  "KEYWORD",     "KEYWORD", "IDENT",    "PUNCT",    "IDENT",
      "OPERATOR", "NEWLINE", "KEYWORD",  "KEYWORD",  "IDENT",       "PUNCT",   "IDENT",    "PUNCT",    "IDENT",
      "OPERATOR", "PUNCT",   "PUNCT",    "IDENT",    "PUNCT",       "IDENT",   "OPERATOR", "IDENT",    "OPERATOR",
      "IDENT",    "KEYWORD", "IDENT",    "PUNCT",    "PUNCT",       "NEWLINE", "KEYWORD",  "IDENT",    "PUNCT",
      "IDENT",    "PUNCT",   "IDENT",    "OPERATOR", "PUNCT",       "PUNCT",   "IDENT",    "PUNCT",    "NEWLINE",
      "KEYWORD",  "IDENT",   "PUNCT",    "IDENT",    "OPERATOR",    "IDENT",   "OPERATOR", "IDENT",    "OPERATOR",
      "IDENT",    "PUNCT",   "PUNCT",    "OPERATOR", "LITERAL:Int", "NEWLINE", "IDENT",    "OPERATOR", "IDENT",
      "PUNCT",    "IDENT",   "OPERATOR", "IDENT",    "PUNCT",       "PUNCT",   "PUNCT",    "NEWLINE",  "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, SafeAdd) {
  const std::string src = R"OVUM(pure fun SafeAdd(a: Int?, b: Int?): Int {
val aVal: int = a ?: 0
val bVal: int = b ?: 0
return Int(aVal + bVal)
})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "pure", "fun", "SafeAdd", "(",   "a",    ":",   "Int",    "?",   ",", "b",    ":", "Int",  "?",   ")",    ":",
      "Int",  "{",   "\\n",     "val", "aVal", ":",   "int",    "=",   "a", "?:",   "0", "\\n",  "val", "bVal", ":",
      "int",  "=",   "b",       "?:",  "0",    "\\n", "return", "Int", "(", "aVal", "+", "bVal", ")",   "\\n",  "}"};
  std::vector<std::string> expected_types = {
      "KEYWORD", "KEYWORD", "IDENT", "PUNCT",    "IDENT",    "PUNCT", "IDENT",    "OPERATOR",    "PUNCT",
      "IDENT",   "PUNCT",   "IDENT", "OPERATOR", "PUNCT",    "PUNCT", "IDENT",    "PUNCT",       "NEWLINE",
      "KEYWORD", "IDENT",   "PUNCT", "IDENT",    "OPERATOR", "IDENT", "OPERATOR", "LITERAL:Int", "NEWLINE",
      "KEYWORD", "IDENT",   "PUNCT", "IDENT",    "OPERATOR", "IDENT", "OPERATOR", "LITERAL:Int", "NEWLINE",
      "KEYWORD", "IDENT",   "PUNCT", "IDENT",    "OPERATOR", "IDENT", "PUNCT",    "NEWLINE",     "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, UnsafeNullable) {
  const std::string src = R"OVUM(fun UnsafeNullable(ptr: Pointer?): Void {
    unsafe {
if (ptr != null) {
val bytes: ByteArray = (ptr as ByteArray)
} }})OVUM";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {
      "fun", "UnsafeNullable", "(", "ptr", ":", "Pointer", "?", ")", ":", "Void", "{",
      "\\n",
      "unsafe", "{",
      "\\n",
      "if", "(", "ptr", "!=", "null", ")", "{",
      "\\n",
      "val", "bytes", ":", "ByteArray", "=", "(", "ptr", "as", "ByteArray", ")",
      "\\n",
      "}",
      "}",
      "}"
  };
  std::vector<std::string> expected_types = {
      "KEYWORD", "IDENT",   "PUNCT",   "IDENT",   "PUNCT",   "IDENT",   "OPERATOR", "PUNCT",    "PUNCT",    "IDENT",
      "PUNCT",   "NEWLINE", "KEYWORD", "PUNCT",   "NEWLINE", "KEYWORD", "PUNCT",    "IDENT",    "OPERATOR", "KEYWORD",
      "PUNCT",   "PUNCT",   "NEWLINE", "KEYWORD", "IDENT",   "PUNCT",   "IDENT",    "OPERATOR", "PUNCT",    "IDENT",
      "KEYWORD", "IDENT",   "PUNCT",   "NEWLINE", "PUNCT",   "PUNCT",   "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
