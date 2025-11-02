#ifndef OVUMC_PREPROCESSORUNITTESTSUITE_HPP_
#define OVUMC_PREPROCESSORUNITTESTSUITE_HPP_

#include <filesystem>
#include <fstream>
#include <expected>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <gtest/gtest.h>
#include "lib/preprocessor/Preprocessor.hpp"

class PreprocessorUnitTestSuite {
public:
  struct TestResult {
    std::string test_name;
    bool passed;
    std::string error_message;
    std::vector<int> included_numbers;
    int expected_include_num;
  };

  static TestResult RunSingleTest(const std::filesystem::path& test_file_path);

private:
  static std::pair<bool, int> ParseTestDirectives(const std::filesystem::path& file_path);
  
  static std::vector<int> ExtractIncludedNumbers(const std::vector<TokenPtr>& tokens);
  
  static bool ValidateIncludedNumbers(const std::vector<int>& included_numbers, int max_include);
  
  static bool IsIncludeToken(const TokenPtr& token);
  
  static bool IsNumberToken(const TokenPtr& token);
  
  static int ExtractNumberFromToken(const TokenPtr& token);
  
  static std::string GetErrorString(const PreprocessorError& error);
};

#endif // OVUMC_PREPROCESSORUNITTESTSUITE_HPP_