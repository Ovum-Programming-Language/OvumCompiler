#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <sstream>

#include "ProjectIntegrationTestSuite.hpp"
#include "lib/compiler_ui/compiler_ui_functions.hpp"
#include "tests/test_functions.hpp"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "tests/test_data"
#endif

void ProjectIntegrationTestSuite::SetUp() {
  std::filesystem::create_directories(kTemporaryDirectoryName);
}

void ProjectIntegrationTestSuite::TearDown() {
  std::filesystem::remove_all(kTemporaryDirectoryName);
}

void ProjectIntegrationTestSuite::CompileAndCompareExample(const std::string& filename_without_extension) {
  std::filesystem::path source_dir = std::filesystem::path(TEST_DATA_DIR) / "examples" / "source";
  std::filesystem::path compiled_dir = std::filesystem::path(TEST_DATA_DIR) / "examples" / "compiled";
  std::filesystem::path include_dir = source_dir / "lib";

  std::filesystem::path source_file = source_dir / (filename_without_extension + ".ovum");
  std::filesystem::path expected_output = compiled_dir / (filename_without_extension + ".oil");
  std::filesystem::path actual_output =
      std::filesystem::path(kTemporaryDirectoryName) / (filename_without_extension + ".oil");

  CompileAndCompareFile(source_file, expected_output, actual_output, include_dir);
}

void ProjectIntegrationTestSuite::CompileAndCompareIntegrational(const std::string& filename_without_extension) {
  std::filesystem::path source_dir = std::filesystem::path(TEST_DATA_DIR) / "integrational" / "positive" / "source";
  std::filesystem::path compiled_dir = std::filesystem::path(TEST_DATA_DIR) / "integrational" / "positive" / "results";
  std::filesystem::path include_dir = source_dir / "include";

  std::filesystem::path source_file = source_dir / (filename_without_extension + ".ovum");
  std::filesystem::path expected_output = compiled_dir / (filename_without_extension + ".oil");
  std::filesystem::path actual_output =
      std::filesystem::path(kTemporaryDirectoryName) / (filename_without_extension + ".oil");

  CompileAndCompareFile(source_file, expected_output, actual_output, include_dir);
}

void ProjectIntegrationTestSuite::CompileAndCompareFile(const std::filesystem::path& source_file,
                                                        const std::filesystem::path& expected_output_file,
                                                        const std::filesystem::path& actual_output_file,
                                                        const std::filesystem::path& include_dir) {
  // Verify source file exists
  ASSERT_TRUE(std::filesystem::exists(source_file)) << "Source file does not exist: " << source_file.string();

  // Verify expected output file exists
  ASSERT_TRUE(std::filesystem::exists(expected_output_file))
      << "Expected output file does not exist: " << expected_output_file.string();

  // Build compiler command
  std::ostringstream cmd;
  cmd << "ovumc -nn -m " << source_file.string() << " -o " << actual_output_file.string();

  if (!include_dir.empty() && std::filesystem::exists(include_dir)) {
    cmd << " -I " << include_dir.string();
  }

  // Run compiler
  std::ostringstream out;
  std::ostringstream err;
  int result = StartCompilerConsoleUI(SplitString(cmd.str()), out, err);

  // Check compilation succeeded
  ASSERT_EQ(result, 0) << "Compilation failed for " << source_file.string() << "\nError output: " << err.str()
                       << "\nStandard output: " << out.str();

  // Verify output file was created
  ASSERT_TRUE(std::filesystem::exists(actual_output_file))
      << "Output file was not created: " << actual_output_file.string();

  // Compare files
  ASSERT_TRUE(CompareFiles(actual_output_file, expected_output_file))
      << "Compiled output does not match expected output for " << source_file.string();
}

bool ProjectIntegrationTestSuite::CompareFiles(const std::filesystem::path& file1, const std::filesystem::path& file2) {
  std::ifstream f1(file1, std::ios::binary);
  std::ifstream f2(file2, std::ios::binary);

  if (!f1.is_open() || !f2.is_open()) {
    return false;
  }

  // Read file contents
  std::string file_1_content((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
  std::string file_2_content((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

  // Normalize line endings by removing all \r characters
  // This handles \r\n (Windows), \n (Unix), and \r (old Mac) line endings
  file_1_content.erase(std::ranges::remove(file_1_content, '\r').begin(), file_1_content.end());
  file_2_content.erase(std::ranges::remove(file_2_content, '\r').begin(), file_2_content.end());

  // Compare normalized contents
  return file_1_content == file_2_content;
}
