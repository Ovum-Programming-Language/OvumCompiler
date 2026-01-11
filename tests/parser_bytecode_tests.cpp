#include <gtest/gtest.h>

#include <iostream>
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
    expr_parser_ = std::make_unique<PrattExpressionParser>(std::move(resolver), factory_, type_parser_.get());
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

    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    if (test_info != nullptr && (diags_.ErrorCount() > 0 || diags_.WarningCount() > 0)) {
      std::cout << "\n=== Parser Diagnostics for test: " << test_info->name() << " ===\n";
      for (const auto& diag : diags_.All()) {
        if (diag.IsSuppressed()) {
          continue;
        }

        std::string severity_str = "UNKNOWN";
        if (diag.GetSeverity()) {
          if (diag.GetSeverity()->Level() >= 3) {
            severity_str = "ERROR";
          } else if (diag.GetSeverity()->Level() >= 2) {
            severity_str = "WARNING";
          } else {
            severity_str = "NOTE";
          }
        }

        std::cout << "[" << severity_str << "]";
        if (!diag.GetCode().empty()) {
          std::cout << " " << diag.GetCode();
        }
        std::cout << ": " << diag.GetMessage();

        if (diag.GetWhere().has_value()) {
          std::cout << " (has location)";
        }
        std::cout << "\n";
      }
      std::cout << "=== End of diagnostics ===\n\n";
    }

    EXPECT_NE(module, nullptr);
    EXPECT_EQ(diags_.ErrorCount(), 0);

    std::ostringstream out;
    BytecodeVisitor visitor(out);
    if (!module) {
      return "";
    }
    module->Accept(visitor);
    std::string bytecode = out.str();

    if (test_info != nullptr) {
      std::cout << "\n=== Bytecode for test: " << test_info->name() << " ===\n";
      std::cout << bytecode;
      std::cout << "\n=== End of bytecode ===\n\n";
    }

    return bytecode;
  }

  DiagnosticCollector diags_;                      // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<ParserFsm> parser_;              // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<IExpressionParser> expr_parser_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<ITypeParser> type_parser_;       // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::shared_ptr<IAstFactory> factory_;           // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};

TEST_F(ParserBytecodeTest, PushInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return 42
}
)");
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return 3.14
}
)");
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushBool) {
  const std::string bc = GenerateBytecode(R"(
fun test(): bool {
    return true
}
)");
  EXPECT_NE(bc.find("PushBool"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushChar) {
  const std::string bc = GenerateBytecode(R"(
fun Main(args: StringArray): int {
    val x: Char = 'a'
    return 0
}
)");
  EXPECT_NE(bc.find("PushChar"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(): byte {
    return 42b
}
)");
  EXPECT_NE(bc.find("PushByte"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushString) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    return "Hello"
}
)");
  EXPECT_NE(bc.find("PushString"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PushNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String? {
    return null
}
)");
  EXPECT_NE(bc.find("PushNull"), std::string::npos);
}

TEST_F(ParserBytecodeTest, Pop) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): Void {
    x + 1
}
)");
  EXPECT_NE(bc.find("Pop"), std::string::npos);
}

TEST_F(ParserBytecodeTest, UnsafeBlock) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    unsafe {
        val x: int = 42
    }
}
)");
  EXPECT_NE(bc.find("UnsafeBlockStart"), std::string::npos);
  EXPECT_NE(bc.find("UnsafeBlockEnd"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CopyAssignment) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public var x: Int;
    public var y: Int;
}

fun test(p1: Point, p2: Point): Void {
    p1 := p2
}
)");
  EXPECT_NE(bc.find("_Point_copy_<M>_Point"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTest, LoadLocal) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): int {
    return x
}
)");
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SetLocal) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    var x: int = 10
    x = 20
    return x
}
)");
  EXPECT_NE(bc.find("SetLocal"), std::string::npos);
}

TEST_F(ParserBytecodeTest, LoadStatic) {
  const std::string bc = GenerateBytecode(R"(
val global: int = 42

fun test(): int {
    return global
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SetStatic) {
  const std::string bc = GenerateBytecode(R"(
val global: int = 42

fun test(): Void {
    global = 100
}
)");
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTest, GlobalVariableInitStatic) {
  const std::string bc = GenerateBytecode(R"(
val x: Int = 42
)");
  EXPECT_NE(bc.find("init-static"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTest, MultipleGlobalVariables) {
  const std::string bc = GenerateBytecode(R"(
val x: Int = 1
val y: Int = 2
val z: Int = 3
)");
  EXPECT_NE(bc.find("init-static"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntArithmetic) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int, b: int): int {
    return a + b - a * b / b % b
}
)");
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("IntSubtract"), std::string::npos);
  EXPECT_NE(bc.find("IntMultiply"), std::string::npos);
  EXPECT_NE(bc.find("IntDivide"), std::string::npos);
  EXPECT_NE(bc.find("IntModulo"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntComparison) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int, b: int): bool {
    return a < b && a <= b && a > 0 && a >= 0 && a == a && a != b
}
)");
  EXPECT_NE(bc.find("IntLessThan"), std::string::npos);
  EXPECT_NE(bc.find("IntLessEqual"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterThan"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterEqual"), std::string::npos);
  EXPECT_NE(bc.find("IntEqual"), std::string::npos);
  EXPECT_NE(bc.find("IntNotEqual"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): int {
    return -x
}
)");
  EXPECT_NE(bc.find("IntNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntBitwiseOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int, b: int): int {
    return a & b | a ^ b << 2 >> 1
}
)");
  EXPECT_NE(bc.find("IntAnd"), std::string::npos);
  EXPECT_NE(bc.find("IntOr"), std::string::npos);
  EXPECT_NE(bc.find("IntXor"), std::string::npos);
  EXPECT_NE(bc.find("IntLeftShift"), std::string::npos);
  EXPECT_NE(bc.find("IntRightShift"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntBitwiseNot) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int): int {
    return ~a
}
)");
  EXPECT_NE(bc.find("IntNot"), std::string::npos);
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
}

TEST_F(ParserBytecodeTest, FloatArithmetic) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: float, b: float): float {
    return a + b * a - b / a
}
)");
  EXPECT_NE(bc.find("FloatAdd"), std::string::npos);
  EXPECT_NE(bc.find("FloatSubtract"), std::string::npos);
  EXPECT_NE(bc.find("FloatMultiply"), std::string::npos);
  EXPECT_NE(bc.find("FloatDivide"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FloatComparison) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: float, b: float): bool {
    return a < b && a <= b && a > 0.0 && a >= 0.0 && a == a && a != b
}
)");
  EXPECT_NE(bc.find("FloatLessThan"), std::string::npos);
  EXPECT_NE(bc.find("FloatLessEqual"), std::string::npos);
  EXPECT_NE(bc.find("FloatGreaterThan"), std::string::npos);
  EXPECT_NE(bc.find("FloatGreaterEqual"), std::string::npos);
  EXPECT_NE(bc.find("FloatEqual"), std::string::npos);
  EXPECT_NE(bc.find("FloatNotEqual"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FloatNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): float {
    return -x
}
)");
  EXPECT_NE(bc.find("FloatNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteArithmetic) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte, b: byte): byte {
    return a + b - a * b / b % b
}
)");
  EXPECT_NE(bc.find("ByteAdd"), std::string::npos);
  EXPECT_NE(bc.find("ByteSubtract"), std::string::npos);
  EXPECT_NE(bc.find("ByteMultiply"), std::string::npos);
  EXPECT_NE(bc.find("ByteDivide"), std::string::npos);
  EXPECT_NE(bc.find("ByteModulo"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteComparison) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte, b: byte): bool {
    return a < b && a <= b && a > 0 && a >= 0 && a == a && a != b
}
)");
  EXPECT_NE(bc.find("ByteLessThan"), std::string::npos);
  EXPECT_NE(bc.find("ByteLessEqual"), std::string::npos);
  EXPECT_NE(bc.find("ByteGreaterThan"), std::string::npos);
  EXPECT_NE(bc.find("ByteGreaterEqual"), std::string::npos);
  EXPECT_NE(bc.find("ByteEqual"), std::string::npos);
  EXPECT_NE(bc.find("ByteNotEqual"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte): byte {
    return -a
}
)");
  EXPECT_NE(bc.find("ByteNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteBitwiseOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte, b: byte): byte {
    return a & b | a ^ b << 2 >> 1
}
)");
  EXPECT_NE(bc.find("ByteAnd"), std::string::npos);
  EXPECT_NE(bc.find("ByteOr"), std::string::npos);
  EXPECT_NE(bc.find("ByteXor"), std::string::npos);
  EXPECT_NE(bc.find("ByteLeftShift"), std::string::npos);
  EXPECT_NE(bc.find("ByteRightShift"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteBitwiseNot) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte): byte {
    return ~~a
}
)");
  EXPECT_NE(bc.find("ByteNot"), std::string::npos);
}

TEST_F(ParserBytecodeTest, BoolLogicalOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: bool, b: bool): bool {
    return a && b || !a
}
)");
  EXPECT_NE(bc.find("BoolAnd"), std::string::npos);
  EXPECT_NE(bc.find("BoolOr"), std::string::npos);
  EXPECT_NE(bc.find("BoolNot"), std::string::npos);
}

TEST_F(ParserBytecodeTest, BoolXor) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: bool, b: bool): bool {
    return a ^ b
}
)");
  EXPECT_NE(bc.find("BoolXor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringConcat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    val a: String = "Hello"
    val b: String = "World"
    return a + " " + b
}
)");
  EXPECT_NE(bc.find("StringConcat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringLength) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): int {
    return s.Length()
}
)");
  EXPECT_NE(bc.find("String_Length"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringSubstring) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String, start: int, len: int): String {
    return s.Substring(start, len)
}
)");
  EXPECT_NE(bc.find("String_Substring"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringCompare) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: String, b: String): int {
    return a.Compare(b)
}
)");
  EXPECT_NE(bc.find("String_Compare"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntToFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): float {
    return x as float
}
)");
  EXPECT_NE(bc.find("IntToFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FloatToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): int {
    return x as int
}
)");
  EXPECT_NE(bc.find("FloatToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: byte): int {
    return x as int
}
)");
  EXPECT_NE(bc.find("ByteToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CharToByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(c: char): byte {
    return c as byte
}
)");
  EXPECT_NE(bc.find("CharToByte"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteToChar) {
  const std::string bc = GenerateBytecode(R"(
fun test(b: byte): char {
    return b as char
}
)");
  EXPECT_NE(bc.find("ByteToChar"), std::string::npos);
}

TEST_F(ParserBytecodeTest, BoolToByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(b: bool): byte {
    return b as byte
}
)");
  EXPECT_NE(bc.find("BoolToByte"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): int {
    return s as int
}
)");
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringToFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): float {
    return s as float
}
)");
  EXPECT_NE(bc.find("StringToFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntToString) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): String {
    return x as String
}
)");
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FloatToString) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): String {
    return x as String
}
)");
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
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
}

TEST_F(ParserBytecodeTest, NestedIf) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int, y: int): int {
    if (x > 0) {
        if (y > 0) {
            return 1
        } else {
            return 2
        }
    } else {
        return 3
    }
}
)");
  EXPECT_NE(bc.find("if"), std::string::npos);
  EXPECT_NE(bc.find("else"), std::string::npos);
}

TEST_F(ParserBytecodeTest, WhileLoop) {
  const std::string bc = GenerateBytecode(R"(
fun f(): Void {
  while (true) {
    break;
    continue;
  }
}
)");
  EXPECT_NE(bc.find("while {"), std::string::npos);
  EXPECT_NE(bc.find("Break"), std::string::npos);
  EXPECT_NE(bc.find("Continue"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ForLoop) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): int {
    var sum: int = 0
    for (item in arr) {
        sum = sum + item
    }
    return sum
}
)");
  EXPECT_NE(bc.find("while"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ForLoopWithBreak) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): int {
    var sum: int = 0
    for (item in arr) {
        if (item > 10) {
            break
        }
        sum = sum + item
    }
    return sum
}
)");
  EXPECT_NE(bc.find("while"), std::string::npos);
  EXPECT_NE(bc.find("Break"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ForLoopWithContinue) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): int {
    var sum: int = 0
    for (item in arr) {
        if (item < 0) {
            continue
        }
        sum = sum + item
    }
    return sum
}
)");
  EXPECT_NE(bc.find("while"), std::string::npos);
  EXPECT_NE(bc.find("Continue"), std::string::npos);
}

TEST_F(ParserBytecodeTest, NestedForLoops) {
  const std::string bc = GenerateBytecode(R"(
fun test(rows: IntArrayArray): int {
    var sum: int = 0
    for (row in rows) {
        for (item in row) {
            sum = sum + item
        }
    }
    return sum
}
)");
  EXPECT_NE(bc.find("while"), std::string::npos);
}

TEST_F(ParserBytecodeTest, Return) {
  const std::string bc = GenerateBytecode(R"(
fun main(): Void {
  return;
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTest, MultipleReturnPaths) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): int {
    if (x < 0) {
        return -1
    }
    if (x == 0) {
        return 0
    }
    return 1
}
)");
  EXPECT_NE(bc.find("if"), std::string::npos);
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTest, GetField) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public val x: Int;
    public val y: Int;
}

fun test(p: Point): int {
    return p.x + p.y
}
)");
  EXPECT_NE(bc.find("GetField"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SetField) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public var x: Int;
    public var y: Int;
}

fun test(p: Point, x: int, y: int): Void {
    p.x = x
    p.y = y
}
)");
  EXPECT_NE(bc.find("SetField"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CallConstructor) {
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

fun test(): Point {
    return Point(10, 20)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CallVirtual) {
  const std::string bc = GenerateBytecode(R"(
interface IBase {
    fun Method(): Void
}

class Base implements IBase {
    public override fun Method(): Void {}
}

class Derived implements IBase {
    public override fun Method(): Void {}
}

fun test(obj: IBase): Void {
    obj.Method()
}
)");
  EXPECT_NE(bc.find("CallVirtual"), std::string::npos);
}

TEST_F(ParserBytecodeTest, Unwrap) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: Int?): int {
    return x!
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ClassDeclaration) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;
  public val y: Int;
}
)");
  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
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
}

TEST_F(ParserBytecodeTest, GetIndex) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int): int {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("_IntArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SetIndex) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int, value: int): Void {
    arr[i] = value
}
)");
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IsNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): bool {
    return x == null
}
)");
  EXPECT_NE(bc.find("IsNull"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IsNotNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): bool {
    return x != null
}
)");
  EXPECT_NE(bc.find("IsNull"), std::string::npos);
  EXPECT_NE(bc.find("BoolNot"), std::string::npos);
}

TEST_F(ParserBytecodeTest, NullCoalesce) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): String {
    return x ?: "default"
}
)");
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SafeCall) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): String? {
    return x?.ToString()
}
)");
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
}

TEST_F(ParserBytecodeTest, TypeTest) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: Object): bool {
    return x is String
}
)");
  EXPECT_NE(bc.find("IsType"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CastAs) {
  GenerateBytecode(R"(
fun test(x: Object): String {
    return x as String
}
)");
}

TEST_F(ParserBytecodeTest, TypeOf) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): String {
    return sys::TypeOf(x)
}
)");
  EXPECT_NE(bc.find("TypeOf"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SizeOf) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public val x: Int;
    public val y: Int;
}

fun test(): int {
    return sys::SizeOf(Point)
}
)");
  EXPECT_NE(bc.find("SizeOf"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FunctionCall) {
  const std::string bc = GenerateBytecode(R"(
fun add(a: int, b: int): int {
    return a + b
}

fun test(x: int, y: int): int {
    val sum: int = add(x, y)
    return sum
}
)");
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_NE(bc.find("add"), std::string::npos);
}

TEST_F(ParserBytecodeTest, MethodCall) {
  const std::string bc = GenerateBytecode(R"(
class Calculator {
    fun Add(a: int, b: int): int {
        return a + b
    }
}

fun test(calc: Calculator, x: int, y: int): int {
    val sum: int = calc.Add(x, y)
    return sum
}
)");
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_NE(bc.find("Add"), std::string::npos);
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
}

TEST_F(ParserBytecodeTest, FunctionNameMangling) {
  const std::string bc = GenerateBytecode(R"(
fun Add(a: Int, b: Int): Int {
  return a + b;
}
)");
  EXPECT_NE(bc.find("_Global_Add_Int_Int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexExpression) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int, b: int, c: int, d: int): int {
    return (a + b) * (c - d) / (a % b)
}
)");
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("IntSubtract"), std::string::npos);
  EXPECT_NE(bc.find("IntMultiply"), std::string::npos);
  EXPECT_NE(bc.find("IntDivide"), std::string::npos);
  EXPECT_NE(bc.find("IntModulo"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexControlFlow) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int, y: int): int {
    if (x > 0) {
        if (y > 0) {
            return x + y
        } else {
            return x - y
        }
    } else {
        return 0
    }
}
)");
  EXPECT_NE(bc.find("if"), std::string::npos);
  EXPECT_NE(bc.find("else"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterThan"), std::string::npos);
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexLoopOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): int {
    var sum: int = 0
    var i: int = 0
    while (i < arr.Length()) {
        sum = sum + arr[i]
        i = i + 1
        if (sum > 100) {
            break
        }
        if (i % 2 == 0) {
            continue
        }
    }
    return sum
}
)");
  EXPECT_NE(bc.find("while"), std::string::npos);
  EXPECT_NE(bc.find("Break"), std::string::npos);
  EXPECT_NE(bc.find("Continue"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexNestedStructures) {
  const std::string bc = GenerateBytecode(R"(
class Node {
    public val value: Int;
    public val children: NodeArray;
}

fun test(root: Node): int {
    var sum: int = 0
    if (root != null) {
        sum = sum + root.value
        for (child in root.children) {
            sum = sum + test(child)
        }
    }
    return sum
}
)");
  EXPECT_NE(bc.find("if"), std::string::npos);
  EXPECT_NE(bc.find("while"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexTypeConversions) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int, y: float, z: byte, c: char, b: bool): Void {
    val f: float = x as float
    val i: int = y as int
    val bi: int = z as int
    val bc: byte = c as byte
    val cb: char = z as char
    val bb: byte = b as byte
    val s1: String = x as String
    val s2: String = y as String
}
)");
  EXPECT_NE(bc.find("IntToFloat"), std::string::npos);
  EXPECT_NE(bc.find("FloatToInt"), std::string::npos);
  EXPECT_NE(bc.find("ByteToInt"), std::string::npos);
  EXPECT_NE(bc.find("CharToByte"), std::string::npos);
  EXPECT_NE(bc.find("ByteToChar"), std::string::npos);
  EXPECT_NE(bc.find("BoolToByte"), std::string::npos);
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexStringOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(s1: String, s2: String, start: int, len: int): Void {
    val concat: String = s1 + s2
    val length: int = s1.Length()
    val substr: String = s1.Substring(start, len)
    val cmp: int = s1.Compare(s2)
    val i: int = s1 as int
    val f: float = s1 as float
    val s3: String = 42 as String
    val s4: String = 3.14 as String
}
)");
  EXPECT_NE(bc.find("StringConcat"), std::string::npos);
  EXPECT_NE(bc.find("String_Length"), std::string::npos);
  EXPECT_NE(bc.find("String_Substring"), std::string::npos);
  EXPECT_NE(bc.find("String_Compare"), std::string::npos);
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
  EXPECT_NE(bc.find("StringToFloat"), std::string::npos);
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexNullableOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?, y: String?): String {
    if (x == null) {
        return y ?: "default"
    }
    val result: String? = x?.ToString()
    return result ?: "empty"
}
)");
  EXPECT_NE(bc.find("IsNull"), std::string::npos);
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexObjectOperations) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public var x: Int;
    public var y: Int;
    
    public fun Point(x: int, y: int): Point {
        this.x = x
        this.y = y
        return this
    }
}

fun test(): int {
    val p: Point = Point(10, 20)
    p.x = 30
    val x: int = p.x
    return x
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("SetField"), std::string::npos);
  EXPECT_NE(bc.find("GetField"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ComplexArrayOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int, j: int, value: int): int {
    arr[i] = value
    val result: int = arr[j]
    return result
}
)");
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemPrintLine) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    sys::PrintLine("Hello")
}
)");
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemPrint) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    sys::Print("Hello")
}
)");
  EXPECT_NE(bc.find("Print"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemReadLine) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    return sys::ReadLine()
}
)");
  EXPECT_NE(bc.find("ReadLine"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemReadInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return sys::ReadInt()
}
)");
  EXPECT_NE(bc.find("ReadInt"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemReadFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return sys::ReadFloat()
}
)");
  EXPECT_NE(bc.find("ReadFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, SystemSqrt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): float {
    return sys::Sqrt(x)
}
)");
  EXPECT_NE(bc.find("sys::Sqrt"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CallIndirect) {
  const std::string bc = GenerateBytecode(R"(
fun test(f: Function<int, int>, x: int): int {
    return f(x)
}
)");
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PureFunction) {
  const std::string bc = GenerateBytecode(R"(
pure fun Hash(x: int): int {
    return x * 31
}
)");
  EXPECT_NE(bc.find("pure"), std::string::npos);
  EXPECT_NE(bc.find("int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PureFunctionWithReturnType) {
  const std::string bc = GenerateBytecode(R"(
pure fun Compute(a: Int, b: Int): Int {
    return a + b
}
)");
  EXPECT_NE(bc.find("pure"), std::string::npos);
  EXPECT_NE(bc.find("Int"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveWrapperInt) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Int, b: Int): Int {
    return a + b
}
)");
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveWrapperFloat) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Float, b: Float): Float {
    return a * b
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("FloatMultiply"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveWrapperByte) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Byte, b: Byte): Byte {
    return a & b
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("ByteAnd"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveIntVsWrapper) {
  const std::string bc = GenerateBytecode(R"(
fun f1(a: int, b: int): int {
    return a + b
}

fun f2(a: Int, b: Int): Int {
    return a + b
}
)");
  EXPECT_NE(bc.find("_Global_f1_int_int"), std::string::npos);
  EXPECT_NE(bc.find("_Global_f2_Int_Int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, IntArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int): int {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("IntArray"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, FloatArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: FloatArray, i: int): float {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("FloatArray"), std::string::npos);
  EXPECT_NE(bc.find("_FloatArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ByteArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: ByteArray, i: int): byte {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("ByteArray"), std::string::npos);
  EXPECT_NE(bc.find("_ByteArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, BoolArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: BoolArray, i: int): bool {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("BoolArray"), std::string::npos);
  EXPECT_NE(bc.find("_BoolArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CharArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: CharArray, i: int): char {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("CharArray"), std::string::npos);
  EXPECT_NE(bc.find("_CharArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ObjectArray) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public val x: Int;
}

fun test(arr: ObjectArray, i: int): Object {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("ObjectArray"), std::string::npos);
  EXPECT_NE(bc.find("_ObjectArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, StringArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: StringArray, i: int): String {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("StringArray"), std::string::npos);
  EXPECT_NE(bc.find("_StringArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ArraySetOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int, value: int): Void {
    arr[i] = value
}
)");
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTest, UnwrapOperation) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public val x: Int;
}

fun test(p: Point): int {
    return p.x
}
)");
  EXPECT_NE(bc.find("GetField"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveWrapperWithLiteral) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Int): Int {
    return a + 10
}
)");
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 10"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, PrimitiveWrapperMixedTypes) {
  const std::string bc = GenerateBytecode(R"(
fun f1(a: Int, b: int): Int {
    return a + b
}

fun f2(a: int, b: Int): Int {
    return a + b
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ArrayOfPrimitiveWrappers) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: ObjectArray, i: int): Int {
    val obj: Object = arr[i]
    return obj as Int
}
)");
  EXPECT_NE(bc.find("_ObjectArray_GetAt_<C>_int"), std::string::npos);
}

// Array Creation Tests
TEST_F(ParserBytecodeTest, CreateIntArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): IntArray {
    return IntArray(10, -1)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_int_int"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 10"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 1"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateFloatArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): FloatArray {
    return FloatArray(5, 0.0)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_FloatArray_int_float"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 5"), std::string::npos);
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateByteArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): ByteArray {
    return ByteArray(8, 0)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_ByteArray_int_byte"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 8"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 0"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateBoolArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): BoolArray {
    return BoolArray(3, false)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_BoolArray_int_bool"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 3"), std::string::npos);
  EXPECT_NE(bc.find("PushBool false"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateCharArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): CharArray {
    return CharArray(4, ' ')
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_CharArray_int_char"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 4"), std::string::npos);
  EXPECT_NE(bc.find("PushChar"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateStringArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): StringArray {
    return StringArray(5, "Null")
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_StringArray_int_String"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 5"), std::string::npos);
  EXPECT_NE(bc.find("PushString \"Null\""), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateObjectArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(): ObjectArray {
    return ObjectArray(3, null)
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_ObjectArray_int_Object"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 3"), std::string::npos);
  EXPECT_NE(bc.find("PushNull"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateArrayWithVariable) {
  const std::string bc = GenerateBytecode(R"(
fun test(size: int): IntArray {
    val arr: IntArray = IntArray(size, 0)
    return arr
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_int_int"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 0"), std::string::npos);
}

TEST_F(ParserBytecodeTest, CreateArrayAndInitialize) {
  const std::string bc = GenerateBytecode(R"(
fun test(): IntArray {
    val arr: IntArray = IntArray(3, 0)
    arr[0] = 10
    arr[1] = 20
    arr[2] = 30
    return arr
}
)");
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_int_int"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 10"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 20"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 30"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ForGC) {
  GenerateBytecode(R"OVUM(
class Point implements IStringConvertible {
    public var X: Float
    public var Y: Float

    public fun Point(x: Float, y: Float): Point {
        this.X = x
        this.Y = y
        return this
    }

    public override fun ToString(): String {
        return "(" + this.X.ToString() + ", " + this.Y.ToString() + ")"
    }
}

class Interval implements IStringConvertible {
    public var Start: Point
    public var End: Point

    public fun Interval(start: Point, end: Point): Interval {
        this.Start = start
        this.End = end
        return this
    }

    public override fun ToString(): String {
        return "[" + this.Start.ToString() + " - " + this.End.ToString() + "]"
    }
}

fun Main(args: StringArray): int {
    val intervals: ObjectArray = ObjectArray(10, Interval(Point(Float(0.0), Float(0.0)), Point(Float(0.0), Float(0.0))))
    sys::PrintLine("Initial intervals:")
    val default_interval : Interval = Interval(Point(-1.0, -1.0), Point(-1.0, -1.0))

    for (interval_obj in intervals) {
        if (!interval_obj is Interval) {
            sys::PrintLine("Unexpected type\n")
            return 1
        }
        val interval_null : Interval? = interval_obj as Interval
        val interval: Interval = interval_null ?: default_interval
        sys::PrintLine(interval.ToString())
    }

    sys::PrintLine("\n")

    var outer: Int = Int(0)
    while (outer < 4) {
        sys::PrintLine("Iteration " + outer.ToString() + ":")
        sys::PrintLine("\n")

        var i: int = 0
        while (i < intervals.Length()) {
            val start: Point = Point(sys::RandomFloatRange(0.0, 100.0), sys::RandomFloatRange(0.0, 100.0))
            val end: Point = Point(sys::RandomFloatRange(0.0, 100.0), sys::RandomFloatRange(0.0, 100.0))
            intervals[i] = Interval(start, end)
            i = i + 1
        }

        val default_interval : Interval = Interval(Point(-1.0, -1.0), Point(-1.0, -1.0))

        for (interval_obj in intervals) {
            if (!interval_obj is Interval) {
                sys::PrintLine("Unexpected type\n")
                return 1
            }
            val interval_null : Interval? = interval_obj as Interval
            val interval: Interval = interval_null ?: default_interval
            sys::PrintLine(interval.ToString())
        }

        outer = outer + 1
    }
    return 0
}
)OVUM");
}
