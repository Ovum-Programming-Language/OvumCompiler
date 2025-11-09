#include "PreprocessorUnitTestSuite.hpp"

#include <expected>
#include <filesystem>
#include <fstream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "lib/lexer/Lexer.hpp"
#include "lib/preprocessor/Preprocessor.hpp"

constexpr size_t kMaxTokenSequenceLength = 17;

namespace ovum::compiler::preprocessor {

void PreprocessorUnitTestSuite::RunSingleTest(const std::filesystem::path& input_file,
                                              const std::filesystem::path& expected_file) {
  TestResult result;
  result.test_name = input_file.filename().string();

  auto expected_tokens_result = TokenizeExpectedFile(expected_file);
  if (!expected_tokens_result.has_value()) {
    result.passed = false;
    result.error_message = "Could not tokenize expected file: " + expected_tokens_result.error();
    ASSERT_TRUE(false) << "Test " << result.test_name << " failed: " << result.error_message;
    return;
  }
  result.expected_tokens = std::move(expected_tokens_result.value());

  PreprocessingParameters params;
  params.main_file = input_file;
  params.include_paths = {
      input_file.parent_path(),
      std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "for_import",
  };

  Preprocessor preprocessor(params);

  auto process_result = preprocessor.Process();

  if (result.expected_tokens[0]->GetLexeme() != "EXPECTERROR") {
    if (!process_result.has_value()) {
      result.passed = false;
      result.error_message = "Preprocessing failed: " + GetErrorString(process_result.error());
      ASSERT_TRUE(false) << "Test " << result.test_name << " failed: " << result.error_message;
      return;
    }

    result.actual_tokens = std::move(process_result.value());
    result.passed = CompareTokenSequences(result.actual_tokens, result.expected_tokens);

    if (!result.passed) {
      result.error_message = "Token sequences do not match";
      std::string detailed_error = BuildDetailedComparison(result.actual_tokens, result.expected_tokens);
      ASSERT_TRUE(false) << "Test " << result.test_name << " failed:\n" << detailed_error;
    }
  } else {
    if (process_result.has_value()) {
      result.passed = false;
      result.error_message = "Preprocessing failed: result has tokens but must be error";
      ASSERT_TRUE(false) << "Test " << result.test_name << " failed: " << result.error_message;
      return;
    }
  }
}

std::expected<std::vector<TokenPtr>, std::string> PreprocessorUnitTestSuite::TokenizeExpectedFile(
    const std::filesystem::path& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    return std::unexpected("Could not open file: " + file_path.string());
  }

  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  ovum::compiler::lexer::Lexer lexer(content, false);
  auto tokens_result = lexer.Tokenize();

  if (!tokens_result) {
    return std::unexpected("Lexer error: " + std::string(tokens_result.error().what()));
  }

  return tokens_result.value();
}

bool PreprocessorUnitTestSuite::CompareTokenSequences(const std::vector<TokenPtr>& actual,
                                                      const std::vector<TokenPtr>& expected) {
  if (actual.size() != expected.size()) {
    return false;
  }

  for (size_t i = 0; i < actual.size(); ++i) {
    if (actual[i]->GetStringType() != expected[i]->GetStringType()) {
      return false;
    }

    if (actual[i]->GetLexeme() != expected[i]->GetLexeme()) {
      return false;
    }
  }

  return true;
}

std::string PreprocessorUnitTestSuite::BuildDetailedComparison(const std::vector<TokenPtr>& actual,
                                                               const std::vector<TokenPtr>& expected) {
  std::stringstream ss;
  ss << "Token sequence comparison failed:\n";

  size_t max_size = std::max(actual.size(), expected.size());

  ss << "Pos | Actual (" << actual.size() << " tokens) | Expected (" << expected.size() << " tokens)\n";
  ss << "----|-------------------|------------------------------\n";

  for (size_t i = 0; i < max_size; ++i) {
    ss << std::setw(3) << i << " | ";

    // Actual token
    if (i < actual.size()) {
      ss << std::setw(kMaxTokenSequenceLength) << TokenToString(actual[i]);
    } else {
      ss << std::setw(kMaxTokenSequenceLength) << "MISSING";
    }

    ss << " | ";

    // Expected token
    if (i < expected.size()) {
      ss << std::setw(kMaxTokenSequenceLength) << TokenToString(expected[i]);
    } else {
      ss << std::setw(kMaxTokenSequenceLength) << "MISSING";
    }

    // Mark differences
    if (i >= actual.size() || i >= expected.size() || !TokensEqual(actual[i], expected[i])) {
      ss << "  <--- DIFFERENCE";
    }

    ss << "\n";
  }

  return ss.str();
}

bool PreprocessorUnitTestSuite::TokensEqual(const TokenPtr& a, const TokenPtr& b) {
  return a->GetStringType() == b->GetStringType() && a->GetLexeme() == b->GetLexeme();
}

std::string PreprocessorUnitTestSuite::TokenToString(const TokenPtr& token) {
  std::stringstream ss;
  ss << "[" << token->GetStringType() << ": '" << token->GetLexeme() << "']";
  return ss.str();
}

std::string PreprocessorUnitTestSuite::GetErrorString(const PreprocessorError& error) {
  return error.what();
}

std::string PreprocessorUnitTestSuite::TokensToString(const std::vector<TokenPtr>& tokens) {
  std::string result;
  for (const auto& token : tokens) {
    if (!result.empty()) {
      result += " ";
    }
    result += token->GetLexeme();
  }
  return result;
}

} // namespace ovum::compiler::preprocessor
