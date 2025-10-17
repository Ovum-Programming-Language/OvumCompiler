#ifndef OVUMC_LEXERUNITTESTSUITE_HPP
#define OVUMC_LEXERUNITTESTSUITE_HPP

#include <gtest/gtest.h>
#include "lib/lexer/handlers/Handler.hpp"

struct LexerUnitTestSuite : public testing::Test {
  const std::string kTemporaryDirectoryName = "./gtest_tmp";

  static std::vector<std::pair<std::string, std::string>> ExtractLexemesAndTypes(const std::vector<TokenPtr>& tokens);

  static void AssertLexemesAndTypesEqual(
      const std::vector<std::pair<std::string, std::string>>& actual,
      const std::vector<std::string>& expected_lexemes,
      const std::vector<std::string>& expected_type_substrs);

  void SetUp() override; // method that is called at the beginning of every test

  void TearDown() override; // method that is called at the end of every test
};


#endif // OVUMC_LEXERUNITTESTSUITE_HPP
