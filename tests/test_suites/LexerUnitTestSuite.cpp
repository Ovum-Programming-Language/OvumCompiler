#include "LexerUnitTestSuite.hpp"

void LexerUnitTestSuite::SetUp() {
  // TODO: Implement test
}

void LexerUnitTestSuite::TearDown() {
  // TODO: Implement test
}

std::vector<std::pair<std::string, std::string>> LexerUnitTestSuite::ExtractLexemesAndTypes(const std::vector<TokenPtr>& tokens) {
  std::vector<std::pair<std::string, std::string>> out;
  if (tokens.empty()) return out;
  // исключаем последний токен (EOF)
  for (size_t i = 0; i + 1 < tokens.size(); ++i) {
    out.emplace_back(tokens[i]->GetLexeme(), tokens[i]->GetStringType());
  }
  return out;
}

void LexerUnitTestSuite::AssertLexemesAndTypesEqual(
    const std::vector<std::pair<std::string, std::string>>& actual,
    const std::vector<std::string>& expected_lexemes,
    const std::vector<std::string>& expected_type_substrs) {
  std::cout << "Exp: ";
  for (const auto& lex : expected_lexemes) {
    std::cout << lex << " ";
  }
  std::cout << std::endl;
  std::cout << "Act: ";
  for (const auto& item : actual) {
    std::cout << item.first << " ";
  }
  std::cout << std::endl;
  std::cout << "Exp: ";
  for (const auto& sub : expected_type_substrs) {
    std::cout << sub << " ";
  }
  std::cout << std::endl;
  // Print actual results
  std::cout << "Act: ";
  for (const auto& item : actual) {
    std::cout << item.second << " ";// "\" (type: \"" << item.second << "\") ";
  }
  std::cout << std::endl;
  ASSERT_EQ(actual.size(), expected_lexemes.size());
  ASSERT_EQ(actual.size(), expected_type_substrs.size());
  for (size_t i = 0; i < actual.size(); ++i) {
    EXPECT_EQ(actual[i].first, expected_lexemes[i]) << "lexeme mismatch at index " << i;
    const auto& type_str = actual[i].second;
    const auto& need_sub = expected_type_substrs[i];
    EXPECT_NE(type_str.find(need_sub), std::string::npos) << i << " " << actual[i].first << " type \"" << type_str << "\" does not contain expected substring \"" << need_sub << "\" for lexeme \"" << actual[i].first << "\"";
  }
}
