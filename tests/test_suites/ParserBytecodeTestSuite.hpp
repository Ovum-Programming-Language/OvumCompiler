#ifndef OVUMC_PARSERBYTECODETESTSUITE_HPP_
#define OVUMC_PARSERBYTECODETESTSUITE_HPP_

#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <tokens/Token.hpp>
#include "lib/parser/ParserFsm.hpp"
#include "lib/parser/ast/IAstFactory.hpp"
#include "lib/parser/diagnostics/DiagnosticCollector.hpp"
#include "lib/parser/pratt/IExpressionParser.hpp"
#include "lib/parser/type_parser/ITypeParser.hpp"

using TokenPtr = ovum::TokenPtr;

class ParserBytecodeTestSuite : public ::testing::Test {
protected:
  void SetUp() override;

  std::unique_ptr<ovum::compiler::parser::Module> Parse(const std::string& code);

  std::string GenerateBytecode(const std::string& code);

  ovum::compiler::parser::DiagnosticCollector
      diags_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<ovum::compiler::parser::ParserFsm>
      parser_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<ovum::compiler::parser::IExpressionParser>
      expr_parser_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::unique_ptr<ovum::compiler::parser::ITypeParser>
      type_parser_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  std::shared_ptr<ovum::compiler::parser::IAstFactory>
      factory_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};

#endif // OVUMC_PARSERBYTECODETESTSUITE_HPP_
