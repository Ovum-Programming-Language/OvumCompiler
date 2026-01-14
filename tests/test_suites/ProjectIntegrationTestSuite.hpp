#ifndef TEMPORARYDIRECTORYTESTSUITE_HPP_
#define TEMPORARYDIRECTORYTESTSUITE_HPP_

#include <filesystem>
#include <string>

#include <gtest/gtest.h>

struct ProjectIntegrationTestSuite : public testing::Test { // special test structure
public:
  const std::string kTemporaryDirectoryName = "./gtest_tmp";

  void SetUp() override; // method that is called at the beginning of every test

  void TearDown() override; // method that is called at the end of every test

  // Compile an example file and compare with expected output
  void CompileAndCompareExample(const std::string& filename_without_extension);

  // Compile an integrational file with include directory and compare with expected output
  void CompileAndCompareIntegrational(const std::string& filename_without_extension);

private:
  // Helper method to compile a file and compare output
  void CompileAndCompareFile(const std::filesystem::path& source_file,
                             const std::filesystem::path& expected_output_file,
                             const std::filesystem::path& actual_output_file,
                             const std::filesystem::path& include_dir = {});

  // Helper method to compare two files
  bool CompareFiles(const std::filesystem::path& file1, const std::filesystem::path& file2);
};

#endif // TEMPORARYDIRECTORYTESTSUITE_HPP_
