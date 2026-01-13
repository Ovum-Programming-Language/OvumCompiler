#include "VisitorTestSuite.hpp"

#include <memory>
#include <string>
#include <unordered_set>

#include <tokens/Token.hpp>
#include "lib/lexer/Lexer.hpp"
#include "lib/parser/ParserFsm.hpp"
#include "lib/parser/ast/BuilderAstFactory.hpp"
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

void VisitorTestSuite::SetUp() {
  factory_ = std::make_shared<BuilderAstFactory>();
  type_parser_ = std::make_unique<QNameTypeParser>(*factory_);
  auto resolver = std::make_unique<DefaultOperatorResolver>();
  expr_parser_ = std::make_unique<PrattExpressionParser>(std::move(resolver), factory_, type_parser_.get());
  parser_ = std::make_unique<ParserFsm>(std::move(expr_parser_), std::move(type_parser_), factory_);
}

std::unique_ptr<Module> VisitorTestSuite::Parse(const std::string& code) {
  diags_.Clear();
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
