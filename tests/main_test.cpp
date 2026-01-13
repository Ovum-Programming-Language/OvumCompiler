#include <filesystem>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>
#include "lib/compiler_ui/compiler_ui_functions.hpp" // include your library here
#include "test_functions.hpp"
#include "test_suites/ProjectIntegrationTestSuite.hpp"

TEST_F(ProjectIntegrationTestSuite, InitTest) {
  ASSERT_TRUE(std::filesystem::is_directory(kTemporaryDirectoryName));
}

// Test help output
TEST_F(ProjectIntegrationTestSuite, HelpOutput) {
  std::ostringstream out;
  std::ostringstream err;

  // Test with -h flag
  std::ostringstream out_h;
  std::ostringstream err_h;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("ovumc -h"), out_h, err_h), 0);
  ASSERT_FALSE(out_h.str().empty());
  ASSERT_TRUE(out_h.str().find("ovumc") != std::string::npos);
  ASSERT_TRUE(out_h.str().find("main-file") != std::string::npos || out_h.str().find("main_file") != std::string::npos);

  // Test with --help flag
  std::ostringstream out_help;
  std::ostringstream err_help;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("ovumc --help"), out_help, err_help), 0);
  ASSERT_FALSE(out_help.str().empty());
  ASSERT_TRUE(out_help.str().find("ovumc") != std::string::npos);
}

// Test wrong arguments
TEST_F(ProjectIntegrationTestSuite, WrongArgsTestNoArguments) {
  std::ostringstream out;
  std::ostringstream err;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("ovumc"), out, err), 1);
  ASSERT_FALSE(err.str().empty());
}

TEST_F(ProjectIntegrationTestSuite, WrongArgsTestInvalidFile) {
  std::ostringstream out;
  std::ostringstream err;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("ovumc -m nonexistent_file.ovum"), out, err), 1);
  ASSERT_FALSE(err.str().empty());
}

TEST_F(ProjectIntegrationTestSuite, WrongArgsTestInvalidIncludeDirectory) {
  // Create a temporary source file for this test
  std::filesystem::path test_file = std::filesystem::path(kTemporaryDirectoryName) / "test.ovum";
  std::ofstream file(test_file);
  file << "fun Main(args: StringArray): int { return 0; }";
  file.close();

  std::ostringstream out;
  std::ostringstream err;
  ASSERT_EQ(
      StartCompilerConsoleUI(SplitString("ovumc -m " + test_file.string() + " -I nonexistent_directory"), out, err), 1);
  ASSERT_FALSE(err.str().empty());
}

TEST_F(ProjectIntegrationTestSuite, WrongArgsTestMissingMainFile) {
  std::ostringstream out;
  std::ostringstream err;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("ovumc -o output.oil"), out, err), 1);
  ASSERT_FALSE(err.str().empty());
}

// Example files compilation tests
TEST_F(ProjectIntegrationTestSuite, ExampleFileChecker) {
  CompileAndCompareExample("checker");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileFact) {
  CompileAndCompareExample("fact");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileFib) {
  CompileAndCompareExample("fib");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileIntervals) {
  CompileAndCompareExample("intervals");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileMemcheck) {
  CompileAndCompareExample("memcheck");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFilePrimes) {
  CompileAndCompareExample("primes");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileShapes) {
  CompileAndCompareExample("shapes");
}

TEST_F(ProjectIntegrationTestSuite, ExampleFileSort) {
  CompileAndCompareExample("sort");
}

// Integrational files compilation tests
TEST_F(ProjectIntegrationTestSuite, IntegrationalFileQuadratic) {
  CompileAndCompareIntegrational("quadratic");
}
