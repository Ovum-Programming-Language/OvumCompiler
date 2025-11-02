#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include "lib/lexer/Lexer.hpp"
#include "test_suites/PreprocessorUnitTestSuite.hpp"
#include "lib/preprocessor/Preprocessor.hpp"

namespace ovum::compiler::preprocessor {

std::filesystem::path get_input_file_path(const std::string& addition_path, const std::string& filename) {
    return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "inputs" / addition_path / filename;
}

std::filesystem::path get_expected_file_path(const std::string& addition_path, const std::string& filename) {
    return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "results" / addition_path / filename;
}

TEST(PreprocessorUnitTestSuite, definebasic) {
  const std::string file_name = "Test01_DefineBasic.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

} // namespace ovum::compiler::preprocessor
