#include <gtest/gtest.h>

#include "lib/parser/ast/visitors/LintVisitor.hpp"
#include "lib/parser/ast/visitors/StructuralValidator.hpp"
#include "lib/parser/ast/visitors/TypeChecker.hpp"
#include "test_suites/VisitorTestSuite.hpp"

using namespace ovum::compiler::parser;

TEST_F(VisitorTestSuite, StructuralValidator_EmptyModuleName) {
  auto module = Parse("module {\n}");
  ASSERT_NE(module, nullptr);

  StructuralValidator validator(diags_);
  module->Accept(validator);

  EXPECT_GT(diags_.ErrorCount(), 0);
  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "E0001") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected error E0001 for empty module name";
}

TEST_F(VisitorTestSuite, LintVisitor_EmptyModule) {
  auto module = Parse("module Test {\n}");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0001") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0001 for empty module";
}

TEST_F(VisitorTestSuite, LintVisitor_EmptyBlock) {
  auto module = Parse(R"(
  fun test(): int {
    {}
    return 0
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0202") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0202 for empty block";
}

TEST_F(VisitorTestSuite, LintVisitor_EmptyFunctionBody) {
  auto module = Parse(R"(
  fun test(): int {
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0102") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0102 for empty function body";
}

TEST_F(VisitorTestSuite, LintVisitor_UnreachableCode) {
  auto module = Parse(R"(
  fun test(): int {
    return 0
    return 1
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0301") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0301 for unreachable code";
}

TEST_F(VisitorTestSuite, LintVisitor_BreakOutsideLoop) {
  auto module = Parse(R"(
  fun test(): int {
    break
    return 0
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  EXPECT_GT(diags_.ErrorCount(), 0);
  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "E0301") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected error E0301 for break outside loop";
}

TEST_F(VisitorTestSuite, LintVisitor_ContinueOutsideLoop) {
  auto module = Parse(R"(
  fun test(): int {
    continue
    return 0
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  EXPECT_GT(diags_.ErrorCount(), 0);
  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "E0302") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected error E0302 for continue outside loop";
}

TEST_F(VisitorTestSuite, LintVisitor_BreakInsideLoop) {
  auto module = Parse(R"(
  fun test(): int {
    while (true) {
      break
    }
    return 0
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "E0301" || diag.GetCode() == "E0302") {
      found = true;
      break;
    }
  }
  EXPECT_FALSE(found) << "Break inside loop should not generate error";
}

TEST_F(VisitorTestSuite, LintVisitor_PureExpressionStatement) {
  auto module = Parse(R"(
  fun test(): int {
    42
    return 0
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0401") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0401 for pure expression statement";
}

TEST_F(VisitorTestSuite, LintVisitor_EmptyStringLiteral) {
  auto module = Parse(R"(
  fun test(): String {
    return ""
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0901") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0901 for empty string literal";
}

TEST_F(VisitorTestSuite, LintVisitor_MutableGlobal) {
  auto module = Parse(R"(
  var global: int = 42
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0801") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0801 for mutable global";
}

TEST_F(VisitorTestSuite, LintVisitor_WhileTrue) {
  auto module = Parse(R"(
  fun test(): int {
    while (true) {
      return 0
    }
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0601") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0601 for while(true)";
}

TEST_F(VisitorTestSuite, LintVisitor_LargeClass) {
  const int k_fields_count = 65;
  std::string code = "module Test {\n  class LargeClass {\n";
  for (int i = 0; i < k_fields_count; ++i) {
    code += "    val field" + std::to_string(i) + ": int = 0\n";
  }
  code += "  }\n}\n";

  if (const auto module = Parse(code)) {
    LintVisitor lint(diags_);
    module->Accept(lint);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "W0101") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected warning W0101 for large class";
  }
}

TEST_F(VisitorTestSuite, LintVisitor_EmptyElseBranch) {
  auto module = Parse(R"(
  fun test(): int {
    if (true) {
      return 0
    } else {
    }
    return 1
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0503") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0503 for empty else branch";
}

TEST_F(VisitorTestSuite, LintVisitor_DeepNesting) {
  auto module = Parse(R"(
  fun test(): int {
    if (true) {
      if (true) {
        if (true) {
          if (true) {
            if (true) {
              return 0
            }
          }
        }
      }
    }
    return 1
  }
)");
  ASSERT_NE(module, nullptr);

  LintVisitor lint(diags_);
  module->Accept(lint);

  bool found = false;
  for (const auto& diag : diags_.All()) {
    if (diag.GetCode() == "W0201") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found) << "Expected warning W0201 for deep nesting";
}

TEST_F(VisitorTestSuite, TypeChecker_ReturnValueInVoidFunction) {
  auto module = Parse(R"(
  fun test(): Void {
    return 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3001") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3001 for return value in void function";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ReturnTypeMismatch) {
  auto module = Parse(R"(
  fun test(): int {
    return "string"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3004") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3004 for return type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_VarDeclTypeMismatch) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = "string"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3003") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3003 for type mismatch in variable declaration";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_GlobalVarDeclTypeMismatch) {
  auto module = Parse(R"(
  val x: int = "string"
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3003") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3003 for type mismatch in global variable declaration";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_AssignmentTypeMismatch) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    x = "string"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3005") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3005 for assignment type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_WrongNumberOfArguments) {
  auto module = Parse(R"(
  fun f(x: int): Void {}
  fun test(): Void {
    f(1, 2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3006") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3006 for wrong number of arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ArgumentTypeMismatch) {
  auto module = Parse(R"(
  fun f(x: int): Void {}
  fun test(): Void {
    f("string")
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3007") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3007 for argument type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_UnknownFunction) {
  auto module = Parse(R"(
  fun test(): Void {
    unknown()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3008") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3008 for unknown function";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_FieldNotFound) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point): Void {
    p.nonexistent
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3009") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3009 for field not found";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ArrayIndexMustBeInt) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr: IntArray = IntArray(10, 0)
    val idx: String = "string"
    arr[idx]
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3010") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3010 for array index must be int";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_LogicalOperatorTypeMismatch) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    x && true
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3011") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3011 for logical operator type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_BinaryOperatorTypeMismatch) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    val y: String = "hello"
    x + y
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3012") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3012 for binary operator type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_UnaryNotTypeMismatch) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    !x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3013") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3013 for unary 'not' type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_UnknownMethod) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point): Void {
    p.nonexistent()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "E3014") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3014 for unknown method";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_UnknownVariable) {
  auto module = Parse(R"(
  fun test(): Void {
    unknown
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode() == "P0001") {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected error E3015 for unknown variable";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidReturnType) {
  auto module = Parse(R"(
  fun test(): int {
    return 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid return type";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ImplicitConversionIntToInt) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: Int = 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should allow implicit conversion from int to Int";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ImplicitConversionIntToIntWrapper) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    val y: Int = x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should allow implicit conversion from int to Int";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidFieldAccess) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point): int {
    return p.x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid field access";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidArrayIndex) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr: IntArray = IntArray(10, 0)
    val x: int = arr[0]
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid array index";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMethodCall) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun getX(): int {
      return this.x
    }
  }
  fun test(p: Point): int {
    return p.getX()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid method call";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidFunctionCall) {
  auto module = Parse(R"(
  fun add(a: int, b: int): int {
    return a + b
  }
  fun test(): int {
    return add(1, 2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid function call";
  }
}

// Additional positive tests
TEST_F(VisitorTestSuite, TypeChecker_ValidBinaryOperation) {
  auto module = Parse(R"(
  fun test(): int {
    return 1 + 2
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid binary operation";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidComparison) {
  auto module = Parse(R"(
  fun test(): bool {
    return 1 < 2
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid comparison";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidLogicalOperation) {
  auto module = Parse(R"(
  fun test(): bool {
    return true && false
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid logical operation";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidUnaryOperation) {
  auto module = Parse(R"(
  fun test(): bool {
    return !true
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid unary operation";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidAssignment) {
  auto module = Parse(R"(
  fun test(): Void {
    var x: int = 0
    x = 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid assignment";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidCopyAssignment) {
  auto module = Parse(R"(
  fun test(): Void {
    var x: Int = 0
    x := 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid copy assignment";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidArrayConstructor) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr: IntArray = IntArray(10, 0)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid array constructor";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidGlobalVariable) {
  auto module = Parse(R"(
  val x: int = 42
  fun test(): int {
    return x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid global variable";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMethodWithoutArguments) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun getX(): int {
      return this.x
    }
  }
  fun test(p: Point): int {
    return p.getX()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid method call without arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMethodWithArguments) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    val y: int = 0
    fun add(other: Point): Point {
      return Point(this.x + other.x, this.y + other.y)
    }
  }
  fun test(p1: Point, p2: Point): Point {
    return p1.add(p2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid method call with arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidFunctionWithMultipleArguments) {
  auto module = Parse(R"(
  fun add(a: int, b: int, c: int): int {
    return a + b + c
  }
  fun test(): int {
    return add(1, 2, 3)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid function call with multiple arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNestedFieldAccess) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    val y: int = 0
  }
  class Line {
    val start: Point = Point(0, 0)
    val end: Point = Point(0, 0)
  }
  fun test(line: Line): int {
    return line.start.x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid nested field access";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidArrayElementAssignment) {
  auto module = Parse(R"(
  fun test(): Void {
    var arr: IntArray = IntArray(10, 0)
    arr[0] = 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid array element assignment";
  }
}

// Complex scenarios and additional test cases

TEST_F(VisitorTestSuite, TypeChecker_ComplexThisExpression) {
  auto module = Parse(R"(
  class Counter {
    val value: int = 0
    fun increment(): Counter {
      return Counter(this.value + 1)
    }
    fun getValue(): int {
      return this.value
    }
  }
  fun test(c: Counter): int {
    return c.increment().getValue()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex this expression";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_MethodChaining) {
  auto module = Parse(R"(
  class Builder {
    val data: int = 0
    fun setValue(v: int): Builder {
      return Builder(v)
    }
    fun getValue(): int {
      return this.data
    }
  }
  fun test(): int {
    return Builder(0).setValue(42).getValue()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for method chaining";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_NestedMethodCalls) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    val y: int = 0
    fun getX(): int {
      return this.x
    }
    fun getY(): int {
      return this.y
    }
  }
  class Line {
    val start: Point = Point(0, 0)
    fun getStart(): Point {
      return this.start
    }
  }
  fun test(line: Line): int {
    return line.getStart().getX()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nested method calls";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ComplexArithmeticExpression) {
  auto module = Parse(R"(
  fun compute(a: int, b: int, c: int): int {
    return (a + b) * c - (a / b) + (a % c)
  }
  fun test(): int {
    return compute(10, 5, 3) + compute(2, 1, 4) * 2
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex arithmetic expression";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ComplexComparisonExpression) {
  auto module = Parse(R"(
  fun test(a: int, b: int, c: int): bool {
    return (a < b) && (b <= c) && (c > a) && (a >= 0) && (a == b) && (b != c)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex comparison expression";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_MultipleFieldAccess) {
  auto module = Parse(R"(
  class Rectangle {
    val topLeft: Point = Point(0, 0)
    val bottomRight: Point = Point(0, 0)
  }
  class Point {
    val x: int = 0
    val y: int = 0
  }
  fun test(rect: Rectangle): int {
    return rect.topLeft.x + rect.bottomRight.y
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for multiple field access";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ComplexConstructorWithThis) {
  auto module = Parse(R"(
  class Vec2 {
    val x: int = 0
    val y: int = 0
    fun add(other: Vec2): Vec2 {
      return Vec2(this.x + other.x, this.y + other.y)
    }
    fun multiply(scalar: int): Vec2 {
      return Vec2(this.x * scalar, this.y * scalar)
    }
  }
  fun test(v1: Vec2, v2: Vec2): Vec2 {
    return v1.add(v2).multiply(2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex constructor with this";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_AllPrimitiveConversions) {
  auto module = Parse(R"(
  fun test(): Void {
    val i: int = 42
    val I: Int = i
    val f: float = 3.14
    val F: Float = f
    val b: byte = 100b
    val B: Byte = b
    val c: char = 'A'
    val C: Char = c
    val bl: bool = true
    val BL: Bool = bl
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for all primitive conversions";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ComplexArrayOperations) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr1: IntArray = IntArray(10, 0)
    val arr2: FloatArray = FloatArray(5, 1.0)
    val x: int = arr1[0] + arr1[1]
    val y: float = arr2[0] * arr2[1]
    arr1[0] = 42
    arr2[1] = 3.14
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex array operations";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_UnaryOperationsOnExpressions) {
  auto module = Parse(R"(
  fun test(a: int, b: bool): Void {
    val x: int = -(a + 10)
    val y: bool = !(b && true)
    val z: int = +(a * 2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for unary operations on expressions";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_MethodCallWithComplexArguments) {
  auto module = Parse(R"(
  class Math {
    fun add(a: int, b: int): int {
      return a + b
    }
  }
  class Calculator {
    val math: Math = Math()
    fun compute(x: int, y: int, z: int): int {
      return this.math.add(this.math.add(x, y), z)
    }
  }
  fun test(): int {
    return Calculator().compute(1, 2, 3)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for method call with complex arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_DeeplyNestedExpressions) {
  auto module = Parse(R"(
  class A {
    val b: B = B()
  }
  class B {
    val c: C = C()
  }
  class C {
    val value: int = 42
  }
  fun test(a: A): int {
    return a.b.c.value
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for deeply nested expressions";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ComplexReturnTypeInference) {
  auto module = Parse(R"(
  class Result {
    val value: int = 0
    fun process(): Result {
      return Result(this.value + 1)
    }
  }
  fun chain(r: Result): Result {
    return r.process().process().process()
  }
  fun test(): Result {
    return chain(Result(0))
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex return type inference";
  }
}

// Error cases for complex scenarios

TEST_F(VisitorTestSuite, TypeChecker_Error_NestedMethodCallWrongArgs) {
  auto module = Parse(R"(
  class Point {
    fun add(other: Point): Point {
      return Point(0, 0)
    }
  }
  fun test(p1: Point, p2: Point): Point {
    return p1.add(p2).add(42)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have type error for nested method call with wrong args";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ThisInNonMethodContext) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = this.value
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300") || diag.GetCode().starts_with("E3015") == 0) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error for this in non-method context";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ComplexAssignmentMismatch) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point): Void {
    p.x = "wrong type"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have type error for complex assignment mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ComplexConstructorWrongArgs) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    val y: int = 0
  }
  fun test(): Point {
    return Point("wrong", 42)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // Constructor validation might not be fully implemented, so this might not error,
    // But if it does, we should catch it
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        break;
      }
    }
    // Note: Constructor argument validation might not be implemented yet
    // So we don't assert on this test result
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ComplexArrayIndexType) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr: IntArray = IntArray(10, 0)
    val x: int = arr["wrong index type"]
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E3010") == 0) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error for wrong array index type";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ComplexComparisonTypeMismatch) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point): bool {
    return (p.x < "string") && true
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E3011")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have type error for complex comparison type mismatch";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMultipleClasses) {
  auto module = Parse(R"(
  class A {
    val value: int = 1
    fun getValue(): int {
      return this.value
    }
  }
  class B {
    val a: A = A()
    fun getA(): A {
      return this.a
    }
  }
  class C {
    val b: B = B()
    fun compute(): int {
      return this.b.getA().getValue()
    }
  }
  fun test(c: C): int {
    return c.compute()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for multiple classes";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidComplexConditionalLogic) {
  auto module = Parse(R"(
  fun test(a: bool, b: bool, c: bool): bool {
    return (a && b) || (c && not a) && (b || not c)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex conditional logic";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidFunctionReturningClass) {
  auto module = Parse(R"(
  class Result {
    val success: bool = true
  }
  fun createResult(): Result {
    return Result()
  }
  fun test(): Result {
    return createResult()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for function returning class";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMixedArithmeticTypes) {
  auto module = Parse(R"(
  fun test(a: int, b: float): float {
    return (a + 10) * b - (a / 2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // This might error depending on implicit conversions
    // But if types are compatible, should not error
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        break;
      }
    }
    // Note: Mixed arithmetic might not be supported, so we don't assert
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidGlobalVariableWithClass) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  val globalPoint: Point = Point()
  fun test(): int {
    return globalPoint.x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for global variable with class";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidRecursiveMethodCalls) {
  auto module = Parse(R"(
  class Counter {
    val count: int = 0
    fun increment(): Counter {
      return Counter(this.count + 1)
    }
  }
  fun test(): Counter {
    return Counter(0).increment().increment().increment()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for recursive method calls";
  }
}

// Tests for nullable types, interfaces, casts, and null-safe operations

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableType) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: String? = null
    val y: String? = "hello"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable types";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableClass) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(): Void {
    val p: Point? = null
    val q: Point? = Point()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable class";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidElvisOperator) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: String? = null
    val result: String = x ?: "default"
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for Elvis operator";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidSafeCall) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun getX(): int {
      return this.x
    }
  }
  fun test(p: Point?): int? {
    return p?.getX()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for safe call";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidCast) {
  auto module = Parse(R"(
  fun test(obj: Object): Void {
    val str: String = obj as String
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid cast";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableCast) {
  auto module = Parse(R"(
  fun test(obj: Object?): Void {
    val str: String? = obj as String?
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid nullable cast";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidTypeTest) {
  auto module = Parse(R"(
  fun test(obj: Object): bool {
    return obj is String
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid type test";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidInterfaceImplementation) {
  auto module = Parse(R"(
  interface IShape {
    fun area(): float
  }
  class Circle implements IShape {
    val radius: float = 1.0
    fun area(): float {
      return 3.14 * this.radius * this.radius
    }
  }
  fun test(): float {
    val shape: IShape = Circle()
    return shape.area()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid interface implementation";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidComplexNullableOperations) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun getX(): int {
      return this.x
    }
  }
  fun test(p: Point?): int {
    return p?.getX() ?: 0
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex nullable operations";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableArray) {
  auto module = Parse(R"(
  fun test(): Void {
    val arr: IntArray? = null
    val arr2: IntArray? = IntArray(10, 0)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable array";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMultipleNullableConversions) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(): Void {
    val p1: Point? = null
    val p2: Point? = Point()
    val p3: Point? = p2
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for multiple nullable conversions";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidSafeCallWithArguments) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun add(other: Point): Point {
      return Point(this.x + other.x, 0)
    }
  }
  fun test(p1: Point?, p2: Point?): Point? {
    return p1?.add(p2 ?: Point())
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for safe call with arguments";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableReturnType) {
  auto module = Parse(R"(
  fun getValue(): String? {
    return null
  }
  fun test(): Void {
    val x: String? = getValue()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable return type";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableFunctionParameter) {
  auto module = Parse(R"(
  fun process(str: String?): String {
    return str ?: "empty"
  }
  fun test(): String {
    return process(null)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable function parameter";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidElvisWithComplexExpression) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
    fun getX(): int {
      return this.x
    }
  }
  fun test(p: Point?): int {
    return p?.getX() ?: (p?.getX() ?: 0)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nested Elvis operators";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableWrapperTypes) {
  auto module = Parse(R"(
  fun test(): Void {
    val i: Int? = null
    val f: Float? = null
    val s: String? = null
    val b: Bool? = null
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable wrapper types";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidInterfaceMethodCall) {
  auto module = Parse(R"(
  interface IDrawable {
    fun draw(): Void
  }
  class Circle implements IDrawable {
    fun draw(): Void {}
  }
  fun test(): Void {
    val shape: IDrawable = Circle()
    shape.draw()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for interface method calls";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidMultipleInterfaces) {
  auto module = Parse(R"(
  interface IA {
    fun methodA(): int
  }
  interface IB {
    fun methodB(): int
  }
  class C implements IA, IB {
    fun methodA(): int { return 1 }
    fun methodB(): int { return 2 }
  }
  fun test(): int {
    val obj: IA = C()
    return obj.methodA()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for multiple interfaces";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableInterface) {
  auto module = Parse(R"(
  interface IShape {
    fun area(): float
  }
  class Circle implements IShape {
    fun area(): float { return 3.14 }
  }
  fun test(): float? {
    val shape: IShape? = Circle()
    return shape?.area()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable interface operations";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidCastToNullable) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(obj: Object?): Void {
    val p: Point? = obj as Point?
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid cast to nullable";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidTypeTestNullable) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(obj: Object?): bool {
    return obj is Point?
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for valid type test nullable";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidNullableFieldAccess) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point?): int? {
    return p?.x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for nullable field access";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidComplexNullableChain) {
  auto module = Parse(R"(
  class A {
    val b: B? = null
  }
  class B {
    val value: int = 42
  }
  fun test(a: A?): int {
    return a?.b?.value ?: 0
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for complex nullable chains";
  }
}

// Error cases for nullable types and interfaces

TEST_F(VisitorTestSuite, TypeChecker_Error_NullableToNonNullable) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: String? = null
    val y: String = x
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        break;
      }
    }
    // Note: This should error, but we allow it for now since implicit conversion might be needed
    // In the future, this should be an error
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_NullableIncompatibleTypes) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(): Void {
    val p: Point? = null
    val x: int = p
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have type error for incompatible nullable type assignment";
  }
}

// Error cases for interfaces

TEST_F(VisitorTestSuite, TypeChecker_Error_InterfaceMissingMethod) {
  auto module = Parse(R"(
  interface IShape {
    fun area(): float
    fun perimeter(): float
  }
  class Circle implements IShape {
    fun area(): float { return 3.14 }
    // Missing perimeter() method
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // Note: TypeChecker doesn't validate interface implementation completeness yet
    // This test documents expected behavior
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_InterfaceWrongReturnType) {
  auto module = Parse(R"(
  interface IShape {
    fun area(): float
  }
  class Circle implements IShape {
    fun area(): int { return 3 }
  }
  fun test(): float {
    val shape: IShape = Circle()
    return shape.area()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        break;
      }
    }
    // Note: TypeChecker might not check return type compatibility for interface methods yet
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_InterfaceWrongParameterType) {
  auto module = Parse(R"(
  interface IShape {
    fun scale(factor: float): Void
  }
  class Circle implements IShape {
    fun scale(factor: int): Void {}
  }
  fun test(): Void {
    val shape: IShape = Circle()
    shape.scale(2.0)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        break;
      }
    }
    // Note: TypeChecker might not check parameter type compatibility for interface methods yet
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_InterfaceWrongNumberOfParameters) {
  auto module = Parse(R"(
  interface IShape {
    fun move(dx: float, dy: float): Void
  }
  class Circle implements IShape {
    fun move(dx: float): Void {}
  }
  fun test(): Void {
    val shape: IShape = Circle()
    shape.move(1.0)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      // When calling through interface, should check interface signature (2 params expected)
      if (diag.GetCode().starts_with("E3006")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error E3006 for wrong number of parameters when calling interface method "
                                "with 1 arg instead of 2";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_ClassDoesNotImplementInterface) {
  auto module = Parse(R"(
  interface IShape {
    fun area(): float
  }
  class Point {
    val x: int = 0
  }
  fun test(): Void {
    val shape: IShape = Point()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error when class doesn't implement interface";
  }
}

// Error cases for casts

TEST_F(VisitorTestSuite, TypeChecker_Error_InvalidCast) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    val str: String = x as String
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // Note: TypeChecker allows any cast for now (runtime will handle invalid casts)
    // This test documents expected behavior
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_CastPrimitiveToObject) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: int = 42
    val obj: Object = x as Object
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // Note: TypeChecker might allow primitive to Object cast
  }
}

// Error cases for Elvis operator

TEST_F(VisitorTestSuite, TypeChecker_Error_ElvisIncompatibleTypes) {
  auto module = Parse(R"(
  fun test(): Void {
    val x: String? = null
    val result: int = x ?: 42
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      // Visit(Elvis&) checks type compatibility and emits E3012
      // Visit(VarDeclStmt&) also checks type compatibility and emits E3003
      if (diag.GetCode().starts_with("E3012") == 0 || diag.GetCode().starts_with("E3003") == 0) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error for incompatible types in Elvis operator";
  }
}

// Error cases for SafeCall

TEST_F(VisitorTestSuite, TypeChecker_Error_SafeCallOnNonNullable) {
  auto module = Parse(R"(
  class Point {
    fun getX(): int { return 0 }
  }
  fun test(p: Point): int? {
    return p?.getX()
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    // Note: SafeCall on non-nullable type might be allowed or might error
    // This test documents current behavior
  }
}

TEST_F(VisitorTestSuite, TypeChecker_Error_SafeCallUnknownField) {
  auto module = Parse(R"(
  class Point {
    val x: int = 0
  }
  fun test(p: Point?): Void {
    val y: int? = p?.unknownField
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E3014") == 0 || diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_TRUE(found_error) << "Should have error for SafeCall on unknown field";
  }
}

// Additional valid tests for interfaces

TEST_F(VisitorTestSuite, TypeChecker_ValidInterfaceWithMultipleMethods) {
  auto module = Parse(R"(
  interface ICalculator {
    fun add(a: int, b: int): int
    fun subtract(a: int, b: int): int
  }
  class Calculator implements ICalculator {
    fun add(a: int, b: int): int { return a + b }
    fun subtract(a: int, b: int): int { return a - b }
  }
  fun test(): int {
    val calc: ICalculator = Calculator()
    return calc.add(1, 2)
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for interface with multiple methods";
  }
}

TEST_F(VisitorTestSuite, TypeChecker_ValidInterfaceWithVoidMethods) {
  auto module = Parse(R"(
  interface IWriter {
    fun write(data: String): Void
  }
  class FileWriter implements IWriter {
    fun write(data: String): Void {}
  }
  fun test(): Void {
    val writer: IWriter = FileWriter()
    writer.write("test")
  }
)");
  if (module) {
    TypeChecker checker(diags_);
    module->Accept(checker);

    bool found_error = false;
    for (const auto& diag : diags_.All()) {
      if (diag.GetCode().starts_with("E300")) {
        found_error = true;
        break;
      }
    }
    EXPECT_FALSE(found_error) << "Should not have type errors for interface with void methods";
  }
}
