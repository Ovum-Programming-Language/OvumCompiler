#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

#include "lib/lexer/Lexer.hpp"
#include "lib/parser/ParserFsm.hpp"
#include "lib/parser/ast/BuilderAstFactory.hpp"
#include "lib/parser/ast/visitors/BytecodeVisitor.hpp"
#include "lib/parser/diagnostics/DiagnosticCollector.hpp"
#include "lib/parser/pratt/DefaultOperatorResolver.hpp"
#include "lib/parser/pratt/PrattExpressionParser.hpp"
#include "lib/parser/tokens/token_streams/VectorTokenStream.hpp"
#include "lib/parser/type_parser/QNameTypeParser.hpp"
#include "lib/preprocessor/directives_processor/TokenDirectivesProcessor.hpp"

using namespace ovum::compiler::parser;
using namespace ovum::compiler::lexer;
using namespace ovum::compiler::preprocessor;

using TokenPtr = ovum::TokenPtr;

class ParserBytecodeTest : public ::testing::Test {
protected:
  void SetUp() override {
    factory_ = std::make_shared<BuilderAstFactory>();
    type_parser_ = std::make_unique<QNameTypeParser>(*factory_);
    auto resolver = std::make_unique<DefaultOperatorResolver>();
    expr_parser_ = std::make_unique<PrattExpressionParser>(std::move(resolver), factory_);
    parser_ = std::make_unique<ParserFsm>(std::move(expr_parser_), std::move(type_parser_), factory_);
  }

  std::unique_ptr<Module> Parse(const std::string& code) {
    Lexer lexer(code, false);
    auto tokens_result = lexer.Tokenize();
    if (!tokens_result.has_value()) {
      return nullptr;
    }

    std::vector<TokenPtr> tokens = std::move(tokens_result.value());

    std::unordered_set<std::string> predefined;
    TokenDirectivesProcessor directives(predefined);
    auto processed = directives.Process(tokens);
    if (!processed.has_value()) {
      return nullptr;
    }

    VectorTokenStream stream(processed.value());
    return parser_->Parse(stream, diags_);
  }

  std::string GenerateBytecode(const std::string& code) {
    auto module = Parse(code);
    EXPECT_NE(module, nullptr);
    EXPECT_EQ(diags_.ErrorCount(), 0);

    std::ostringstream out;
    BytecodeVisitor visitor(out);
    module->Accept(visitor);
    return out.str();
  }

  DiagnosticCollector diags_;
  std::unique_ptr<ParserFsm> parser_;
  std::unique_ptr<IExpressionParser> expr_parser_;
  std::unique_ptr<ITypeParser> type_parser_;
  std::shared_ptr<IAstFactory> factory_;
};

TEST_F(ParserBytecodeTest, EmptyFunction) {
  const std::string bc = GenerateBytecode(R"(
fun main(): Void {
  return;
}
)");

  EXPECT_NE(bc.find("function:"), std::string::npos);
  EXPECT_NE(bc.find("Return"), std::string::npos);

  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, MainFunction) {
  const std::string bc = GenerateBytecode(R"(
fun Main(argc: int): int {
  return argc + 10000000;
}
)");

  EXPECT_NE(bc.find("function:"), std::string::npos);
  EXPECT_NE(bc.find("Return"), std::string::npos);

  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, FunctionArgumentOrder) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Int, b: Int): Int {
  return a + b;
}
)");

  auto pos_b = bc.find("LoadLocal 1");
  auto pos_a = bc.find("LoadLocal 0");
  auto pos_add = bc.find("IntAdd");

  ASSERT_NE(pos_b, std::string::npos);
  ASSERT_NE(pos_a, std::string::npos);
  ASSERT_NE(pos_add, std::string::npos);

  EXPECT_LT(pos_b, pos_a);
  EXPECT_LT(pos_a, pos_add);

  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, FunctionNameMangling) {
  const std::string bc = GenerateBytecode(R"(
fun Add(a: Int, b: Int): Int {
  return a + b;
}
)");

  EXPECT_NE(bc.find("_Global_Add_Int_Int"), std::string::npos);

  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, IfStatement) {
  const std::string bc = GenerateBytecode(R"(
fun f(x: Int): Void {
  if (x > 0) {
    return;
  }
}
)");

  EXPECT_NE(bc.find("if {"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterThan"), std::string::npos);

  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, IfElseStatement) {
  const std::string bc = GenerateBytecode(R"(
fun f(x: Int): Void {
  if (x > 0) {
    return;
  } else {
    return;
  }
}
)");

  EXPECT_NE(bc.find("else {"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, WhileLoop) {
  const std::string bc = GenerateBytecode(R"(
fun f(): Void {
  while (true) {
    break;
  }
}
)");

  EXPECT_NE(bc.find("while {"), std::string::npos);
  EXPECT_NE(bc.find("Break"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, ComparisonOrder) {
  const std::string bc = GenerateBytecode(R"(
fun f(x: Int): Bool {
  return x <= 5;
}
)");

  auto pos_5 = bc.find("PushInt 5");
  auto pos_x = bc.find("LoadLocal 0");
  auto pos_cmp = bc.find("IntLessEqual");

  ASSERT_NE(pos_5, std::string::npos);
  ASSERT_NE(pos_x, std::string::npos);
  ASSERT_NE(pos_cmp, std::string::npos);

  EXPECT_LT(pos_5, pos_x);
  EXPECT_LT(pos_x, pos_cmp);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, NullableReturn) {
  const std::string bc = GenerateBytecode(R"(
fun f(): String? {
  return null;
}
)");

  EXPECT_NE(bc.find("PushNull"), std::string::npos);
  EXPECT_NE(bc.find("Return"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, GlobalVariableInitStatic) {
  const std::string bc = GenerateBytecode(R"(
val x: Int = 42;
)");

  EXPECT_NE(bc.find("init-static"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, ClassDeclaration) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;
  public val y: Int;
}
)");

  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, ClassConstructor) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;
  public val y: Int;

  public fun Point(x: int, y: int): Point {
    this.x = x
    this.y = y
    return this
  }
}
)");

  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, ClassDestructor) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;

  public destructor(): Void {
  }
}
)");

  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, InterfaceDeclaration) {
  const std::string bc = GenerateBytecode(R"(
interface Drawable {
  fun Draw(): Void;
}

class Point implements Drawable {
  fun Draw(): Void {}
}
)");

  EXPECT_NE(bc.find("interface"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, MultipleFunctions) {
  const std::string bc = GenerateBytecode(R"(
fun a(): Void { return; }
fun b(): Void { return; }
)");

  EXPECT_NE(bc.find("a"), std::string::npos);
  EXPECT_NE(bc.find("b"), std::string::npos);
  std::cout << bc << std::endl;
}

TEST_F(ParserBytecodeTest, Fibbonacci) {
  const std::string bc = GenerateBytecode(R"(
pure fun Fib(n: int): int {
    if (n <= 1) return n
    val fib1: int = Fib(n - 1)
    val fib2: int = Fib(n - 2)
    return fib1 + fib2
}
)");

  EXPECT_NE(bc.find("Fib"), std::string::npos);
  std::cout << bc << std::endl;
}
