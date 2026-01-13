#include <gtest/gtest.h>

#include "test_suites/ParserBytecodeTestSuite.hpp"

TEST_F(ParserBytecodeTestSuite, PushInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return 42
}
)");
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return 3.14
}
)");
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushBool) {
  const std::string bc = GenerateBytecode(R"(
fun test(): bool {
    return true
}
)");
  EXPECT_NE(bc.find("PushBool"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushChar) {
  const std::string bc = GenerateBytecode(R"(
fun Main(args: StringArray): int {
    val x: Char = 'a'
    return 0
}
)");
  EXPECT_NE(bc.find("PushChar"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(): byte {
    return 42b
}
)");
  EXPECT_NE(bc.find("PushByte"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushString) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    return "Hello"
}
)");
  EXPECT_NE(bc.find("PushString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PushNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String? {
    return null
}
)");
  EXPECT_NE(bc.find("PushNull"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, Pop) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): Void {
    x + 1
}
)");
  EXPECT_NE(bc.find("Pop"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, UnsafeBlock) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    unsafe {
        val x: int = 42
    }
}
)");
  EXPECT_EQ(bc.find("UnsafeBlockStart"), std::string::npos);
  EXPECT_EQ(bc.find("UnsafeBlockEnd"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignment) {
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

TEST_F(ParserBytecodeTestSuite, LoadLocal) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): int {
    return x
}
)");
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SetLocal) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    var x: int = 10
    x = 20
    return x
}
)");
  EXPECT_NE(bc.find("SetLocal"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, LoadStatic) {
  const std::string bc = GenerateBytecode(R"(
global val a: int = 42

fun test(): int {
    return a
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SetStatic) {
  const std::string bc = GenerateBytecode(R"(
global val a: int = 42

fun test(): Void {
    a = 100
}
)");
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SetStaticCallConstructor) {
  const std::string bc = GenerateBytecode(R"(
global val a: Int = 42

fun test(): Void {
    a = 100
}
)");
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalVariableInitStatic) {
  const std::string bc = GenerateBytecode(R"(
val x: Int = 42
)");
  EXPECT_NE(bc.find("init-static"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 42"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, MultipleGlobalVariables) {
  const std::string bc = GenerateBytecode(R"(
val x: Int = 1
val y: Int = 2
val z: Int = 3
)");
  EXPECT_NE(bc.find("init-static"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalFloatWithConstructor) {
  const std::string bc = GenerateBytecode(R"(
global val pi: Float = 3.14159

fun test(): Void {
    pi = 2.718
}
)");
  EXPECT_NE(bc.find("CallConstructor _Float_float"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalVarMutable) {
  const std::string bc = GenerateBytecode(R"(
global var counter: Int = 0

fun increment(): Void {
    counter = counter + 1
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalInExpression) {
  const std::string bc = GenerateBytecode(R"(
global val x: Int = 10
global val y: Int = 20

fun sum(): Int {
    return x + y
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("init-static"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalByteConversion) {
  const std::string bc = GenerateBytecode(R"(
global val b: Byte = 42b

fun test(): Void {
    b = 100b
}
)");
  EXPECT_NE(bc.find("CallConstructor _Byte_byte"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalBoolConversion) {
  const std::string bc = GenerateBytecode(R"(
global val flag: Bool = true

fun test(): Void {
    flag = false
}
)");
  EXPECT_NE(bc.find("CallConstructor _Bool_bool"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalCharConversion) {
  const std::string bc = GenerateBytecode(R"(
global val ch: Char = 'A'

fun test(): Void {
    ch = 'B'
}
)");
  EXPECT_NE(bc.find("CallConstructor _Char_char"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalMultipleFunctions) {
  const std::string bc = GenerateBytecode(R"(
global var value: Int = 0

fun get(): Int {
    return value
}

fun set(x: Int): Void {
    value = x
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalWithArithmetic) {
  const std::string bc = GenerateBytecode(R"(
global val a: Int = 5
global val b: Int = 3

fun compute(): Int {
    return a * b + a - b
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("IntMultiply"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("IntSubtract"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalWithComparison) {
  const std::string bc = GenerateBytecode(R"(
global val threshold: Int = 100

fun check(x: Int): bool {
    return x > threshold
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterThan"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalPrimitiveTypes) {
  const std::string bc = GenerateBytecode(R"(
global val i: int = 42
global val f: float = 3.14
global val b: bool = true

fun test(): Void {
    i = 100
    f = 2.718
    b = false
}
)");
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 100"), std::string::npos);
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
  EXPECT_NE(bc.find("PushBool"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ToStringIntConversion) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): String {
    return ToString(x)
}
)");
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
  EXPECT_EQ(bc.find("Call ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ToStringFloatConversion) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): String {
    return ToString(x)
}
)");
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
  EXPECT_EQ(bc.find("Call ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ToStringByteConversion) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: byte): String {
    return ToString(x)
}
)");
  EXPECT_NE(bc.find("ByteToString"), std::string::npos);
  EXPECT_EQ(bc.find("Call ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ToStringCharConversion) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: char): String {
    return ToString(x)
}
)");
  EXPECT_NE(bc.find("CharToString"), std::string::npos);
  EXPECT_EQ(bc.find("Call ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ToStringBoolConversion) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: bool): String {
    return ToString(x)
}
)");
  EXPECT_NE(bc.find("BoolToString"), std::string::npos);
  EXPECT_EQ(bc.find("Call ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalIntToWrapperConversion) {
  const std::string bc = GenerateBytecode(R"(
global val x: Int = 42

fun test(): Void {
    x = 100
}
)");
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalFloatToWrapperConversion) {
  const std::string bc = GenerateBytecode(R"(
global val x: Float = 3.14

fun test(): Void {
    x = 2.718
}
)");
  EXPECT_NE(bc.find("CallConstructor _Float_float"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalByteToWrapperConversion) {
  const std::string bc = GenerateBytecode(R"(
global val x: Byte = 42b

fun test(): Void {
    x = 100b
}
)");
  EXPECT_NE(bc.find("CallConstructor _Byte_byte"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalCharToWrapperConversion) {
  const std::string bc = GenerateBytecode(R"(
global val x: Char = 'A'

fun test(): Void {
    x = 'B'
}
)");
  EXPECT_NE(bc.find("CallConstructor _Char_char"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalBoolToWrapperConversion) {
  const std::string bc = GenerateBytecode(R"(
global val x: Bool = true

fun test(): Void {
    x = false
}
)");
  EXPECT_NE(bc.find("CallConstructor _Bool_bool"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalIntToStringChain) {
  const std::string bc = GenerateBytecode(R"(
global val value: Int = 42

fun test(): String {
    return ToString(value)
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("Call _Int_ToString_<C>"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalFloatToStringChain) {
  const std::string bc = GenerateBytecode(R"(
global val pi: Float = 3.14159

fun test(): String {
    return ToString(pi)
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("Call _Float_ToString_<C>"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalIntArithmeticWithConversion) {
  const std::string bc = GenerateBytecode(R"(
global val a: Int = 10
global val b: Int = 20

fun test(): Int {
    return a + b
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalIntComparisonWithConversion) {
  const std::string bc = GenerateBytecode(R"(
global val threshold: Int = 100

fun check(x: Int): bool {
    return x > threshold
}
)");
  EXPECT_NE(bc.find("LoadStatic"), std::string::npos);
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterThan"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, GlobalMixedTypesConversion) {
  const std::string bc = GenerateBytecode(R"(
global val count: Int = 0
global val rate: Float = 1.5
global val active: Bool = true

fun update(): Void {
    count = 10
    rate = 2.5
    active = false
}
)");
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Float_float"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Bool_bool"), std::string::npos);
  EXPECT_NE(bc.find("SetStatic"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IntArithmetic) {
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

TEST_F(ParserBytecodeTestSuite, IntComparison) {
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

TEST_F(ParserBytecodeTestSuite, IntNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): int {
    return -x
}
)");
  EXPECT_NE(bc.find("IntNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IntBitwiseOperations) {
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

TEST_F(ParserBytecodeTestSuite, IntBitwiseNot) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: int): int {
    return ~a
}
)");
  EXPECT_NE(bc.find("IntNot"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ComparisonOrder) {
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

TEST_F(ParserBytecodeTestSuite, FloatArithmetic) {
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

TEST_F(ParserBytecodeTestSuite, FloatComparison) {
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

TEST_F(ParserBytecodeTestSuite, FloatNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): float {
    return -x
}
)");
  EXPECT_NE(bc.find("FloatNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ByteArithmetic) {
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

TEST_F(ParserBytecodeTestSuite, ByteComparison) {
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

TEST_F(ParserBytecodeTestSuite, ByteNegate) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte): byte {
    return -a
}
)");
  EXPECT_NE(bc.find("ByteNegate"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ByteBitwiseOperations) {
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

TEST_F(ParserBytecodeTestSuite, ByteBitwiseNot) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: byte): byte {
    return ~~a
}
)");
  EXPECT_NE(bc.find("ByteNot"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, BoolLogicalOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: bool, b: bool): bool {
    return a && b || !a
}
)");
  EXPECT_NE(bc.find("BoolAnd"), std::string::npos);
  EXPECT_NE(bc.find("BoolOr"), std::string::npos);
  EXPECT_NE(bc.find("BoolNot"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, BoolXor) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: bool, b: bool): bool {
    return a ^ b
}
)");
  EXPECT_NE(bc.find("BoolXor"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringConcat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    val a: String = "Hello"
    val b: String = "World"
    return a + " " + b
}
)");
  EXPECT_NE(bc.find("StringConcat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringLength) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): int {
    return s.Length()
}
)");
  EXPECT_NE(bc.find("String_Length"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringSubstring) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String, start: int, len: int): String {
    return s.Substring(start, len)
}
)");
  EXPECT_NE(bc.find("String_Substring"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringCompare) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: String, b: String): int {
    return a.Compare(b)
}
)");
  EXPECT_NE(bc.find("String_Compare"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IntToFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): float {
    return x as float
}
)");
  EXPECT_NE(bc.find("IntToFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, FloatToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): int {
    return x as int
}
)");
  EXPECT_NE(bc.find("FloatToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ByteToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: byte): int {
    return x as int
}
)");
  EXPECT_NE(bc.find("ByteToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CharToByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(c: char): byte {
    return c as byte
}
)");
  EXPECT_NE(bc.find("CharToByte"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ByteToChar) {
  const std::string bc = GenerateBytecode(R"(
fun test(b: byte): char {
    return b as char
}
)");
  EXPECT_NE(bc.find("ByteToChar"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, BoolToByte) {
  const std::string bc = GenerateBytecode(R"(
fun test(b: bool): byte {
    return b as byte
}
)");
  EXPECT_NE(bc.find("BoolToByte"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): int {
    return s as int
}
)");
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringToFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): float {
    return s as float
}
)");
  EXPECT_NE(bc.find("StringToFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IntToString) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): String {
    return x as String
}
)");
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, FloatToString) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): String {
    return x as String
}
)");
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IfStatement) {
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

TEST_F(ParserBytecodeTestSuite, IfElseStatement) {
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

TEST_F(ParserBytecodeTestSuite, NestedIf) {
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

TEST_F(ParserBytecodeTestSuite, WhileLoop) {
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

TEST_F(ParserBytecodeTestSuite, ForLoop) {
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

TEST_F(ParserBytecodeTestSuite, ForLoopWithBreak) {
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

TEST_F(ParserBytecodeTestSuite, ForLoopWithContinue) {
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

TEST_F(ParserBytecodeTestSuite, NestedForLoops) {
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

TEST_F(ParserBytecodeTestSuite, Return) {
  const std::string bc = GenerateBytecode(R"(
fun main(): Void {
  return;
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, MultipleReturnPaths) {
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

TEST_F(ParserBytecodeTestSuite, GetField) {
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

TEST_F(ParserBytecodeTestSuite, SetField) {
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

TEST_F(ParserBytecodeTestSuite, CallConstructor) {
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

TEST_F(ParserBytecodeTestSuite, CallVirtual) {
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

TEST_F(ParserBytecodeTestSuite, Unwrap) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: Int?): int {
    return x!
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ClassDeclaration) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;
  public val y: Int;
}
)");
  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ClassConstructor) {
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

TEST_F(ParserBytecodeTestSuite, ClassDestructor) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  public val x: Int;

  public destructor(): Void {
  }
}
)");
  EXPECT_NE(bc.find("vtable Point"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceDeclaration) {
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

TEST_F(ParserBytecodeTestSuite, GetIndex) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int): int {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("_IntArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SetIndex) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int, value: int): Void {
    arr[i] = value
}
)");
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IsNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): bool {
    return x == null
}
)");
  EXPECT_NE(bc.find("IsNull"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, IsNotNull) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): bool {
    return x != null
}
)");
  EXPECT_NE(bc.find("IsNull"), std::string::npos);
  EXPECT_NE(bc.find("BoolNot"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, NullCoalesce) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): String {
    return x ?: "default"
}
)");
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SafeCall) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): String? {
    return x?.ToString()
}
)");
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, TypeTest) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: Object): bool {
    return x is String
}
)");
  EXPECT_NE(bc.find("IsType"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CastAs) {
  GenerateBytecode(R"(
fun test(x: Object): String {
    return x as String
}
)");
}

TEST_F(ParserBytecodeTestSuite, TypeOf) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: int): String {
    return sys::TypeOf(x)
}
)");
  EXPECT_NE(bc.find("TypeOf"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SizeOf) {
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

TEST_F(ParserBytecodeTestSuite, FunctionCall) {
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

TEST_F(ParserBytecodeTestSuite, MethodCall) {
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

TEST_F(ParserBytecodeTestSuite, FunctionArgumentOrder) {
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

TEST_F(ParserBytecodeTestSuite, FunctionNameMangling) {
  const std::string bc = GenerateBytecode(R"(
fun Add(a: Int, b: Int): Int {
  return a + b;
}
)");
  EXPECT_NE(bc.find("_Global_Add_Int_Int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ComplexExpression) {
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

TEST_F(ParserBytecodeTestSuite, ComplexControlFlow) {
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

TEST_F(ParserBytecodeTestSuite, ComplexLoopOperations) {
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

TEST_F(ParserBytecodeTestSuite, ComplexNestedStructures) {
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

TEST_F(ParserBytecodeTestSuite, ComplexTypeConversions) {
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

TEST_F(ParserBytecodeTestSuite, ComplexStringOperations) {
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

TEST_F(ParserBytecodeTestSuite, ComplexNullableOperations) {
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

TEST_F(ParserBytecodeTestSuite, ComplexObjectOperations) {
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

TEST_F(ParserBytecodeTestSuite, ComplexArrayOperations) {
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

TEST_F(ParserBytecodeTestSuite, SystemPrintLine) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    sys::PrintLine("Hello")
}
)");
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SystemPrint) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    sys::Print("Hello")
}
)");
  EXPECT_NE(bc.find("Print"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SystemReadLine) {
  const std::string bc = GenerateBytecode(R"(
fun test(): String {
    return sys::ReadLine()
}
)");
  EXPECT_NE(bc.find("ReadLine"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SystemReadInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return sys::ReadInt()
}
)");
  EXPECT_NE(bc.find("ReadInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SystemReadFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return sys::ReadFloat()
}
)");
  EXPECT_NE(bc.find("ReadFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SystemSqrt) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: float): float {
    return sys::Sqrt(x + 1.0)
}
)");
  EXPECT_NE(bc.find("FloatSqrt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CallIndirect) {
  const std::string bc = GenerateBytecode(R"(
fun test(f: Function<int, int>, x: int): int {
    return f(x)
}
)");
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PureFunction) {
  const std::string bc = GenerateBytecode(R"(
pure fun Hash(x: int): int {
    return x * 31
}
)");
  EXPECT_NE(bc.find("pure"), std::string::npos);
  EXPECT_NE(bc.find("int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PureFunctionWithReturnType) {
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

TEST_F(ParserBytecodeTestSuite, PrimitiveWrapperInt) {
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

TEST_F(ParserBytecodeTestSuite, PrimitiveWrapperFloat) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Float, b: Float): Float {
    return a * b
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("FloatMultiply"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PrimitiveWrapperByte) {
  const std::string bc = GenerateBytecode(R"(
fun f(a: Byte, b: Byte): Byte {
    return a & b
}
)");
  EXPECT_NE(bc.find("Unwrap"), std::string::npos);
  EXPECT_NE(bc.find("ByteAnd"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, PrimitiveIntVsWrapper) {
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

TEST_F(ParserBytecodeTestSuite, IntArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int): int {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("IntArray"), std::string::npos);
  EXPECT_NE(bc.find("_IntArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, FloatArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: FloatArray, i: int): float {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("FloatArray"), std::string::npos);
  EXPECT_NE(bc.find("_FloatArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ByteArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: ByteArray, i: int): byte {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("ByteArray"), std::string::npos);
  EXPECT_NE(bc.find("_ByteArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, BoolArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: BoolArray, i: int): bool {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("BoolArray"), std::string::npos);
  EXPECT_NE(bc.find("_BoolArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CharArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: CharArray, i: int): char {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("CharArray"), std::string::npos);
  EXPECT_NE(bc.find("_CharArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ObjectArray) {
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

TEST_F(ParserBytecodeTestSuite, StringArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: StringArray, i: int): String {
    return arr[i]
}
)");
  EXPECT_NE(bc.find("StringArray"), std::string::npos);
  EXPECT_NE(bc.find("_StringArray_GetAt_<C>_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ArraySetOperations) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, i: int, value: int): Void {
    arr[i] = value
}
)");
  EXPECT_NE(bc.find("_IntArray_SetAt_<M>_int_int"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, UnwrapOperation) {
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

TEST_F(ParserBytecodeTestSuite, PrimitiveWrapperWithLiteral) {
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

TEST_F(ParserBytecodeTestSuite, PrimitiveWrapperMixedTypes) {
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

TEST_F(ParserBytecodeTestSuite, ArrayOfPrimitiveWrappers) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: ObjectArray, i: int): Int {
    val obj: Object = arr[i]
    return obj as Int
}
)");
  EXPECT_NE(bc.find("_ObjectArray_GetAt_<C>_int"), std::string::npos);
}

// Array Creation Tests
TEST_F(ParserBytecodeTestSuite, CreateIntArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateFloatArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateByteArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateBoolArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateCharArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateStringArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateObjectArray) {
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

TEST_F(ParserBytecodeTestSuite, CreateArrayWithVariable) {
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

TEST_F(ParserBytecodeTestSuite, CreateArrayAndInitialize) {
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

TEST_F(ParserBytecodeTestSuite, ForGC) {
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

TEST_F(ParserBytecodeTestSuite, TypeAlias) {
  const std::string bc = GenerateBytecode(R"(
typealias UserId = Int
typealias UserName = String

fun test(id: UserId, name: UserName): Void {
}
)");
  EXPECT_NE(bc.find("_Global_test_Int_String"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentString) {
  const std::string bc = GenerateBytecode(R"(
fun test(s1: String, s2: String): Void {
    s1 := s2
}
)");
  EXPECT_NE(bc.find("_String_copy_<M>_String"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentArray) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr1: IntArray, arr2: IntArray): Void {
    arr1 := arr2
}
)");
  EXPECT_NE(bc.find("_IntArray_copy_<M>_IntArray"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: Int, b: Int): Void {
    a := b
}
)");
  EXPECT_NE(bc.find("_Int_copy_<M>_Int"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentIntWithExpression) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: Int): Void {
    a := a + 1
}
)");
  EXPECT_NE(bc.find("_Int_copy_<M>_int"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_EQ(bc.find("CallConstructor _Int_int"), std::string::npos);
  EXPECT_NE(bc.find("IntAdd"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: Float, b: Float): Void {
    a := b
}
)");
  EXPECT_NE(bc.find("_Float_copy_<M>_Float"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentFloatWithExpression) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: Float): Void {
    a := a * 2.0
}
)");
  EXPECT_NE(bc.find("_Float_copy_<M>_float"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
  EXPECT_NE(bc.find("FloatMultiply"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ReferenceAssignmentString) {
  const std::string bc = GenerateBytecode(R"(
fun test(a: String, b: String): Void {
    a = b
}
)");
  EXPECT_EQ(bc.find("_String_copy_<M>_String"), std::string::npos);
  EXPECT_NE(bc.find("SetLocal"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentField) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public var x: Int
    public var y: Int
}

fun test(p: Point, v: Int): Void {
    p.x := v
}
)");
  EXPECT_NE(bc.find("SetField"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ReferenceAssignmentField) {
  const std::string bc = GenerateBytecode(R"(
class Point {
    public var x: Int
    public var y: Int
}

fun test(p: Point, v: Int): Void {
    p.x = v
}
)");
  EXPECT_NE(bc.find("SetField"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CopyAssignmentArrayElement) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, v: Int): Void {
    arr[0] := v
}
)");
  EXPECT_NE(bc.find("SetAt"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ReferenceAssignmentArrayElement) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray, v: Int): Void {
    arr[0] = v
}
)");
  EXPECT_NE(bc.find("SetAt"), std::string::npos);
  EXPECT_NE(bc.find("PushInt 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 0"), std::string::npos);
  EXPECT_NE(bc.find("LoadLocal 1"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceIComparable) {
  const std::string bc = GenerateBytecode(R"(
class Point implements IComparable {
    public val X: Int
    public val Y: Int

    public fun Point(x: Int, y: Int): Point {
        this.X = x
        this.Y = y
        return this
    }

    public override fun IsLess(other: Object): Bool {
        return true
    }

    public override fun Equals(other: Object): Bool {
        return true
    }
}
)");
  EXPECT_NE(bc.find("IComparable"), std::string::npos);
  EXPECT_NE(bc.find("IsLess"), std::string::npos);
  EXPECT_NE(bc.find("Equals"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceIHashable) {
  const std::string bc = GenerateBytecode(R"(
class Point implements IHashable {
    public val X: Int
    public val Y: Int

    public fun Point(x: Int, y: Int): Point {
        this.X = x
        this.Y = y
        return this
    }

    public override fun GetHash(): Int {
        return Int(42)
    }
}
)");
  EXPECT_NE(bc.find("IHashable"), std::string::npos);
  EXPECT_NE(bc.find("GetHash"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceIStringConvertible) {
  const std::string bc = GenerateBytecode(R"(
class Point implements IStringConvertible {
    public val X: Int
    public val Y: Int

    public fun Point(x: Int, y: Int): Point {
        this.X = x
        this.Y = y
        return this
    }

    public override fun ToString(): String {
        return "Point"
    }
}
)");
  EXPECT_NE(bc.find("IStringConvertible"), std::string::npos);
  EXPECT_NE(bc.find("ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ArrayLength) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): Int {
    return arr.Length()
}
)");
  EXPECT_NE(bc.find("_IntArray_Length_<C>"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ArrayToString) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr: IntArray): String {
    return arr.ToString()
}
)");
  EXPECT_NE(bc.find("_IntArray_ToString_<C>"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ArrayIsLess) {
  const std::string bc = GenerateBytecode(R"(
fun test(arr1: IntArray, arr2: IntArray): Bool {
    return arr1.IsLess(arr2)
}
)");
  EXPECT_NE(bc.find("_IntArray_IsLess_<M>"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, HexLiteral) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return 0x1A
}
)");
  EXPECT_NE(bc.find("PushInt 26"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, BinaryLiteral) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return 0b1010
}
)");
  EXPECT_NE(bc.find("PushInt 10"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, FloatScientificNotation) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return 2.0e10
}
)");
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, FloatSpecialValues) {
  const std::string bc = GenerateBytecode(R"(
fun test(): float {
    return Infinity
}
)");
  EXPECT_NE(bc.find("PushFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, MultipleInterfaces) {
  const std::string bc = GenerateBytecode(R"(
class Point implements IStringConvertible, IComparable, IHashable {
    public val X: Int
    public val Y: Int

    public fun Point(x: Int, y: Int): Point {
        this.X = x
        this.Y = y
        return this
    }

    public override fun ToString(): String {
        return "Point"
    }

    public override fun IsLess(other: Object): Bool {
        return true
    }

    public override fun Equals(other: Object): Bool {
        return true
    }

    public override fun GetHash(): Int {
        return Int(42)
    }
}
)");
  EXPECT_NE(bc.find("IStringConvertible"), std::string::npos);
  EXPECT_NE(bc.find("IComparable"), std::string::npos);
  EXPECT_NE(bc.find("IHashable"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceImplementationWithMethods) {
  const std::string bc = GenerateBytecode(R"(
interface IShape {
  fun area(): float
  fun perimeter(): float
}

class Circle implements IShape {
  val radius: float = 1.0
  
  fun Circle(r: float): Circle {
    this.radius = r
    return this
  }
  
  fun area(): float {
    return 3.14 * this.radius * this.radius
  }
  
  fun perimeter(): float {
    return 2 * 3.14 * this.radius
  }
}

fun test(): float {
  val shape: IShape = Circle(5.0)
  return shape.area()
}
)");
  EXPECT_NE(bc.find("interface"), std::string::npos);
  EXPECT_NE(bc.find("Circle"), std::string::npos);
  EXPECT_NE(bc.find("area"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, NullableFieldAccess) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  val x: int = 0
}

fun test(p: Point?): int? {
  return p?.x
}
)");
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ElvisOperator) {
  const std::string bc = GenerateBytecode(R"(
fun test(x: String?): String {
  return x ?: "default"
}
)");
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ElvisOperatorWithComplexExpression) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  val x: int = 0
}

fun test(p: Point?): int {
  return p?.x ?: 0
}
)");
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, CastNullableType) {
  const std::string bc = GenerateBytecode(R"(
class Point {
  val x: int = 0
}

fun test(obj: Object?): Point? {
  return obj as Point?
}
)");
  EXPECT_NE(bc.find("CallConstructor _Nullable_Object"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceMethodCall) {
  const std::string bc = GenerateBytecode(R"(
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
  EXPECT_NE(bc.find("CallVirtual"), std::string::npos);
  EXPECT_NE(bc.find("draw"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SafeCallOnNullableInterface) {
  const std::string bc = GenerateBytecode(R"(
interface IShape {
  fun area(): float
}

class Circle implements IShape {
  fun area(): float { return 3.14 }
}

fun test(shape: IShape?): float? {
  return shape?.area()
}
)");
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
  EXPECT_NE(bc.find("area"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, ComplexNullableInterfaceChain) {
  const std::string bc = GenerateBytecode(R"(
interface IShape {
  fun area(): float
}

class Circle implements IShape {
  val radius: float = 1.0
  fun area(): float { return 3.14 * this.radius * this.radius }
}

fun test(shape: IShape?): float {
  return shape?.area() ?: 0.0
}
)");
  EXPECT_NE(bc.find("SafeCall"), std::string::npos);
  EXPECT_NE(bc.find("NullCoalesce"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, TypeTestWithInterface) {
  const std::string bc = GenerateBytecode(R"(
interface IShape {
  fun area(): float
}

class Circle implements IShape {
  fun area(): float { return 3.14 }
}

fun test(obj: Object): bool {
  return obj is IShape
}
)");
  EXPECT_NE(bc.find("IsType"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, EmptyReturnInVoidFunction) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    return
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, EmptyReturnInIfBlock) {
  const std::string bc = GenerateBytecode(R"(
fun test(start: int, end: int): Void {
    if (start >= end) {
        return
    }
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
  EXPECT_NE(bc.find("IntGreaterEqual"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, EmptyReturnWithMultiplePaths) {
  const std::string bc = GenerateBytecode(R"(
fun process(x: int): Void {
    if (x < 0) {
        return
    }
    if (x == 0) {
        return
    }
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, EmptyReturnInMethod) {
  const std::string bc = GenerateBytecode(R"(
class Logger {
    fun log(msg: String): Void {
        return
    }
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, EmptyReturnInIfElse) {
  const std::string bc = GenerateBytecode(R"(
fun check(condition: bool): Void {
    if (condition) {
        return
    } else {
        return
    }
}
)");
  EXPECT_NE(bc.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToStringInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(n: int): Void {
    sys::PrintLine(sys::ToString(n))
}
)");
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToStringFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(n: float): Void {
    sys::PrintLine(sys::ToString(n))
}
)");
  EXPECT_NE(bc.find("FloatToString"), std::string::npos);
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToStringWithFunctionCall) {
  const std::string bc = GenerateBytecode(R"(
pure fun Factorial(n: int): int {
    if (n <= 1) {
        return 1
    }
    return n * Factorial(n - 1)
}

fun test(n: int): Void {
    sys::PrintLine(sys::ToString(Factorial(n)))
}
)");
  EXPECT_NE(bc.find("IntToString"), std::string::npos);
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
  EXPECT_NE(bc.find("Call"), std::string::npos); // Should call Factorial
  EXPECT_EQ(bc.find("Call sys::ToString"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToIntWithArrayAccess) {
  const std::string bc = GenerateBytecode(R"(
fun test(sArr: StringArray): Int {
    return sys::ToInt(sArr[0])
}
)");
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToInt"), std::string::npos);
}
TEST_F(ParserBytecodeTestSuite, SysStringToInt) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): int {
    return sys::ToInt(s)
}
)");
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysStringToFloat) {
  const std::string bc = GenerateBytecode(R"(
fun test(s: String): float {
    return sys::ToFloat(s)
}
)");
  EXPECT_NE(bc.find("StringToFloat"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToFloat"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToIntWithFunctionCall) {
  const std::string bc = GenerateBytecode(R"(
fun test(): int {
    return sys::ToInt(sys::ReadLine())
}
)");
  EXPECT_NE(bc.find("StringToInt"), std::string::npos);
  EXPECT_NE(bc.find("ReadLine"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::ToInt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysSqrtWithCasts) {
  const std::string bc = GenerateBytecode(R"(
fun test(n: int): int {
    var n_sqrt = sys::Sqrt(n as float) as int
    return n_sqrt
}
)");
  EXPECT_NE(bc.find("IntToFloat"), std::string::npos);
  EXPECT_NE(bc.find("FloatSqrt"), std::string::npos);
  EXPECT_NE(bc.find("FloatToInt"), std::string::npos);
  EXPECT_EQ(bc.find("Call sys::Sqrt"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, SysToStringGetMemoryUsage) {
  const std::string bc = GenerateBytecode(R"(
fun test(): Void {
    sys::PrintLine(sys::GetMemoryUsage().ToString())
}
)");
  EXPECT_NE(bc.find("GetMemoryUsage"), std::string::npos);
  EXPECT_NE(bc.find("CallConstructor _Int_int"), std::string::npos);
  EXPECT_NE(bc.find("Call _Int_ToString_<C>"), std::string::npos);
  EXPECT_NE(bc.find("PrintLine"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, StringEqualsCorrectOrderOfArguments) {
  const std::string bc = GenerateBytecode(R"(
fun test(s1: String, s2: String): bool {
    return s1.Equals(s2)
}
)");
  ASSERT_NE(bc.find("LoadLocal 0"), std::string::npos);
  ASSERT_NE(bc.find("LoadLocal 1"), std::string::npos);
  EXPECT_LT(bc.find("LoadLocal 1"), bc.find("LoadLocal 0"));
  EXPECT_NE(bc.find("_String_Equals"), std::string::npos);
  EXPECT_EQ(bc.find("CallVirtual"), std::string::npos);
}

TEST_F(ParserBytecodeTestSuite, InterfaceVirtualMethodCallCorrectOrderOfArguments) {
  const std::string bc = GenerateBytecode(R"(
fun test(lhs : IComparable, rhs : IComparable): bool {
    return lhs.IsLess(rhs)
}
)");
  ASSERT_NE(bc.find("LoadLocal 0"), std::string::npos);
  ASSERT_NE(bc.find("LoadLocal 1"), std::string::npos);
  EXPECT_LT(bc.find("LoadLocal 1"), bc.find("LoadLocal 0"));
  EXPECT_NE(bc.find("_IsLess"), std::string::npos);
  EXPECT_NE(bc.find("CallVirtual"), std::string::npos);
  EXPECT_EQ(bc.find("_IComparable_IsLess"), std::string::npos);
}
