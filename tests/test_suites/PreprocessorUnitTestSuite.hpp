#ifndef OVUMC_PREPROCESSORUNITTESTSUITE_HPP_
#define OVUMC_PREPROCESSORUNITTESTSUITE_HPP_

#include "lib/preprocessor/Preprocessor.hpp"

namespace ovum::compiler::preprocessor {

class PreprocessorUnitTestSuite {
public:
  struct TestResult {
    std::string test_name;
    bool passed;
    std::string error_message;
    std::vector<TokenPtr> actual_tokens;
    std::vector<TokenPtr> expected_tokens;
  };

  static void RunSingleTest(const std::filesystem::path& input_file, const std::filesystem::path& expected_file);

private:
  static std::expected<std::vector<TokenPtr>, std::string> TokenizeExpectedFile(const std::filesystem::path& file_path);

  static bool CompareTokenSequences(const std::vector<TokenPtr>& actual, const std::vector<TokenPtr>& expected);

  static std::string BuildDetailedComparison(const std::vector<TokenPtr>& actual,
                                             const std::vector<TokenPtr>& expected);

  static bool TokensEqual(const TokenPtr& a, const TokenPtr& b);

  static std::string TokenToString(const TokenPtr& token);

  static std::string GetErrorString(const PreprocessorError& error);

  static std::string TokensToString(const std::vector<TokenPtr>& tokens);
};

} // namespace ovum::compiler::preprocessor

#endif // OVUMC_PREPROCESSORUNITTESTSUITE_HPP_
