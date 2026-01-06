#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

#include "lib/lexer/Lexer.hpp"
#include "lib/preprocessor/directives_processor/TokenDirectivesProcessor.hpp"
#include "lib/parser/ParserFsm.hpp"
#include "lib/parser/ast/BuilderAstFactory.hpp"
#include "lib/parser/ast/visitors/BytecodeVisitor.hpp"
#include "lib/parser/diagnostics/DiagnosticCollector.hpp"
#include "lib/parser/pratt/PrattExpressionParser.hpp"
#include "lib/parser/pratt/DefaultOperatorResolver.hpp"
#include "lib/parser/type_parser/QNameTypeParser.hpp"
#include "lib/parser/tokens/token_streams/VectorTokenStream.hpp"

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
    parser_ = std::make_unique<ParserFsm>(std::move(expr_parser_), std::move(type_parser_),
                                          std::shared_ptr<IAstFactory>(factory_));
  }

  std::unique_ptr<Module> ParseCode(const std::string& code) {
    // Tokenize
    Lexer lexer(code, false);
    auto tokens_result = lexer.Tokenize();
    if (!tokens_result.has_value()) {
      return nullptr;
    }
    std::vector<TokenPtr> tokens = std::move(tokens_result.value());
    if (tokens.empty()) {
      return nullptr;
    }

    // Preprocess - process directives only (no file imports in tests)
    std::unordered_set<std::string> predefined_symbols;
    TokenDirectivesProcessor directives_processor(predefined_symbols);
    auto processed_result = directives_processor.Process(tokens);
    if (!processed_result.has_value()) {
      return nullptr;
    }
    tokens = std::move(processed_result.value());

    // Parse
    VectorTokenStream stream(tokens);
    return parser_->Parse(stream, diags_);
  }

  DiagnosticCollector diags_;
  std::unique_ptr<ParserFsm> parser_;
  std::unique_ptr<IExpressionParser> expr_parser_;
  std::unique_ptr<ITypeParser> type_parser_;
  std::shared_ptr<IAstFactory> factory_;
};

TEST_F(ParserBytecodeTest, ParseSimpleFunction) {
  const std::string code = R"(
fun main(): Void {
  return;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
  EXPECT_GT(module->Decls().size(), 0);
  
  // Generate bytecode
  std::ostringstream bytecode_output;
  BytecodeVisitor visitor(bytecode_output);
  module->Accept(visitor);
  
  std::string bytecode = bytecode_output.str();
  EXPECT_NE(bytecode.find("function:0"), std::string::npos);
  EXPECT_NE(bytecode.find("main"), std::string::npos);
  EXPECT_NE(bytecode.find("Return"), std::string::npos);
}

TEST_F(ParserBytecodeTest, ParseFunctionWithReturn) {
  const std::string code = R"(
fun add(a: Int, b: Int): Int {
  return a + b;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseTypeWithArguments) {
  const std::string code = R"(
fun process(list: List<Int>): Void {
  return;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseNestedTypeArguments) {
  const std::string code = R"(
fun process(map: Map<String, List<Int>>): Void {
  return;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseNullableType) {
  const std::string code = R"(
fun getValue(): String? {
  return null;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseClass) {
  const std::string code = R"(
class Point {
  val x: Int;
  val y: Int;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseInterface) {
  const std::string code = R"(
interface Drawable {
  fun draw(): Void;
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseGlobalVariable) {
  const std::string code = R"(
val global: Int = 42;
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseIfStatement) {
  const std::string code = R"(
fun test(x: Int): Void {
  if (x > 0) {
    return;
  }
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

TEST_F(ParserBytecodeTest, ParseWhileLoop) {
  const std::string code = R"(
fun loop(): Void {
  while (true) {
    break;
  }
}
)";

  auto module = ParseCode(code);
  ASSERT_NE(module, nullptr);
  EXPECT_EQ(diags_.ErrorCount(), 0);
}

}  // namespace ovum::compiler::parser

