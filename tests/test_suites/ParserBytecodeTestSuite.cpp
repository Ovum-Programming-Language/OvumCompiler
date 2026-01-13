#include "ParserBytecodeTestSuite.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

#include <tokens/Token.hpp>
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

void ParserBytecodeTestSuite::SetUp() {
  factory_ = std::make_shared<BuilderAstFactory>();
  type_parser_ = std::make_unique<QNameTypeParser>(*factory_);
  auto resolver = std::make_unique<DefaultOperatorResolver>();
  expr_parser_ = std::make_unique<PrattExpressionParser>(std::move(resolver), factory_, type_parser_.get());
  parser_ = std::make_unique<ParserFsm>(std::move(expr_parser_), std::move(type_parser_), factory_);
}

std::unique_ptr<Module> ParserBytecodeTestSuite::Parse(const std::string& code) {
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

std::string ParserBytecodeTestSuite::GenerateBytecode(const std::string& code) {
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
      std::cout << ": " << diag.GetDiagnosticsMessage();

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
