#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <utility>
#include "test_suites/LexerUnitTestSuite.hpp"
#include "lib/lexer/Lexer.hpp"

TEST(LexerUnitTestSuite, EmptyString) {
  const std::string src = "";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {};
  std::vector<std::string> expected_types = {};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, SingleCharacter) {
  const std::string src = "c";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"c"};
  std::vector<std::string> expected_types = {"IDENT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, Keywords) {
  const std::string src = "fun pure val var class interface implements override if else while for in return break continue unsafe is as";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"fun", "pure", "val", "var", "class", "interface", "implements", "override", "if", "else", "while", "for", "in", "return", "break", "continue", "unsafe", "is", "as"};
  std::vector<std::string> expected_types = {"KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD", "KEYWORD"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, PunctColon) {
  const std::string src = ":";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {":"};
  std::vector<std::string> expected_types = {"PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, PunctSemicolon) {
  const std::string src = ";";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {";"};
  std::vector<std::string> expected_types = {"PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, Punctuation) {
  const std::string src = "{ } ( ) [ ] , : ;";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"{", "}", "(", ")", "[", "]", ",", ":", ";"};
  std::vector<std::string> expected_types = {"PUNCT", "PUNCT", "PUNCT", "PUNCT", "PUNCT", "PUNCT", "PUNCT", "PUNCT", "PUNCT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, Identifiers) {
  const std::string src = "abc ABC _abc ovum ExampleFundamentals Void String IntArray";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"abc", "ABC", "_abc", "ovum", "ExampleFundamentals", "Void", "String", "IntArray"};
  std::vector<std::string> expected_types = {"IDENT", "IDENT", "IDENT", "IDENT", "IDENT", "IDENT", "IDENT", "IDENT"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, OperatorsSimple) {
  const std::string src = "+ - * / % < > ! . ?";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"+", "-", "*", "/", "%", "<", ">", "!", ".", "?"};
  std::vector<std::string> expected_types = {"OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, OperatorsMultiChar) {
  const std::string src = "== != <= >= && || = := ::";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"==", "!=", "<=", ">=", "&&", "||", "=", ":=", "::"};
  std::vector<std::string> expected_types = {"OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR", "OPERATOR"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, OperatorsNullSafe) {
  const std::string src = "?. ?:";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"?.", "?:"};
  std::vector<std::string> expected_types = {"OPERATOR", "OPERATOR"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, OperatorXor) {
  const std::string src = "xor";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"xor"};
  std::vector<std::string> expected_types = {"OPERATOR"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, LiteralsInt) {
  const std::string src = "0 42 255";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"0", "42", "255"};
  std::vector<std::string> expected_types = {"LITERAL:Int", "LITERAL:Int", "LITERAL:Int"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, LiteralsFloat) {
  const std::string src = "3.14 1e3 .5 2.0E-2";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"3.14", "1e3", ".5", "2.0E-2"};
  std::vector<std::string> expected_types = {"LITERAL:Float", "LITERAL:Float", "LITERAL:Float", "LITERAL:Float"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, LiteralsChar) {
  const std::string src = "'A' '\\n'";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"'A'", "'\\n'"};
  std::vector<std::string> expected_types = {"LITERAL:Char", "LITERAL:Char"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, LiteralsBool) {
  const std::string src = "true false";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"true", "false"};
  std::vector<std::string> expected_types = {"LITERAL:Bool", "LITERAL:Bool"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, KeywordNull) {
  const std::string src = "null";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"null"};
  std::vector<std::string> expected_types = {"KEYWORD"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, LiteralsString) {
  const std::string src = R"("hello" "world\n" "")";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {R"("hello")", R"("world\n")", R"("")"};
  std::vector<std::string> expected_types = {"LITERAL:String", "LITERAL:String", "LITERAL:String"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, Preprocessor) {
  const std::string src = "#import #define #ifdef";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"#import", "#define", "#ifdef"};
  std::vector<std::string> expected_types = {"KEYWORD", "KEYWORD", "KEYWORD"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}

TEST(LexerUnitTestSuite, Newline) {
  const std::string src = "\n";
  Lexer lexer(src);
  auto tokens = lexer.Tokenize();
  auto items = LexerUnitTestSuite::ExtractLexemesAndTypes(tokens);
  std::vector<std::string> expected_lexemes = {"\\n"};
  std::vector<std::string> expected_types = {"NEWLINE"};
  LexerUnitTestSuite::AssertLexemesAndTypesEqual(items, expected_lexemes, expected_types);
}
