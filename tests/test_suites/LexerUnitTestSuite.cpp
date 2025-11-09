#include "LexerUnitTestSuite.hpp"

#include <iostream>

void LexerUnitTestSuite::SetUp() {
}

void LexerUnitTestSuite::TearDown() {
}

std::vector<std::pair<std::string, std::string>> LexerUnitTestSuite::ExtractLexemesAndTypes(
    const std::vector<ovum::TokenPtr>& tokens) {
  std::vector<std::pair<std::string, std::string>> out;
  if (tokens.empty())
    return out;
  // excluding the last token (EOF)
  for (size_t i = 0; i + 1 < tokens.size(); ++i) {
    out.emplace_back(tokens[i]->GetLexeme(), tokens[i]->GetStringType());
  }
  return out;
}

void LexerUnitTestSuite::PrintLexemesAndTypes(const std::vector<std::pair<std::string, std::string>>& items) {
  std::cout << "Lexemes and Types:" << '\n';
  for (const auto& item : items) {
    std::cout << item.first << " (" << item.second << ")" << '\n';
  }
  std::cout << "End of output" << '\n';
}

void LexerUnitTestSuite::AssertLexemesAndTypesEqual(const std::vector<std::pair<std::string, std::string>>& actual,
                                                    const std::vector<std::string>& expected_lexemes,
                                                    const std::vector<std::string>& expected_type_substrs) {
  bool has_mismatch = false;

  if (actual.size() != expected_lexemes.size() || actual.size() != expected_type_substrs.size()) {
    has_mismatch = true;
  } else {
    for (size_t i = 0; i < actual.size(); ++i) {
      if (actual[i].first != expected_lexemes[i]) {
        has_mismatch = true;
        break;
      }
      const auto& type_str = actual[i].second;
      const auto& need_sub = expected_type_substrs[i];
      if (type_str.find(need_sub) == std::string::npos) {
        has_mismatch = true;
        break;
      }
    }
  }

  if (has_mismatch) {
    std::cout << "Exp lex: ";
    for (const auto& lex : expected_lexemes) {
      std::cout << lex << " ";
    }
    std::cout << '\n';
    std::cout << "Act lex: ";
    for (const auto& item : actual) {
      std::cout << item.first << " ";
    }
    std::cout << '\n';
    std::cout << "Exp typ: ";
    for (const auto& sub : expected_type_substrs) {
      std::cout << sub << " ";
    }
    std::cout << '\n';
    std::cout << "Act typ: ";
    for (const auto& item : actual) {
      std::cout << item.second << " ";
    }
    std::cout << '\n';
  }

  ASSERT_EQ(expected_lexemes.size(), expected_type_substrs.size());
  for (size_t i = 0; i < actual.size() && i < expected_lexemes.size() && i < expected_type_substrs.size(); ++i) {
    EXPECT_EQ(actual[i].first, expected_lexemes[i]) << "lexeme mismatch at index " << i;
    const auto& type_str = actual[i].second;
    const auto& need_sub = expected_type_substrs[i];
    EXPECT_NE(type_str.find(need_sub), std::string::npos)
        << i << " " << actual[i].first << " type \"" << type_str << "\" does not contain expected substring \""
        << need_sub << "\" for lexeme \"" << actual[i].first << "\"";
  }

  ASSERT_EQ(actual.size(), expected_lexemes.size());
  ASSERT_EQ(actual.size(), expected_type_substrs.size());
}
