#ifndef OVUMC_LEXERUNITTESTSUITE_HPP
#define OVUMC_LEXERUNITTESTSUITE_HPP

#include <gtest/gtest.h>

#include <tokens/Token.hpp>

struct LexerUnitTestSuite : public testing::Test {
public:
  static std::vector<std::pair<std::string, std::string>> ExtractLexemesAndTypes(
      const std::vector<ovum::TokenPtr>& tokens);

  static void PrintLexemesAndTypes(const std::vector<std::pair<std::string, std::string>>& items);

  static void AssertLexemesAndTypesEqual(const std::vector<std::pair<std::string, std::string>>& actual,
                                         const std::vector<std::string>& expected_lexemes,
                                         const std::vector<std::string>& expected_type_substrs);

  void SetUp() override; // method that is called at the beginning of every test

  void TearDown() override; // method that is called at the end of every test
};

#endif // OVUMC_LEXERUNITTESTSUITE_HPP
