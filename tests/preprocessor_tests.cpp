#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "test_suites/PreprocessorUnitTestSuite.hpp"

namespace ovum::compiler::preprocessor {

std::filesystem::path GetInputFilePath(const std::string& addition_path, const std::string& filename) {
  return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "inputs" / addition_path / filename;
}

std::filesystem::path GetExpectedFilePath(const std::string& addition_path, const std::string& filename) {
  return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "results" / addition_path / filename;
}

// Define tests
TEST(PreprocessorUnitTestSuite, DefineBasic) {
  const std::string file_name = "Test01_DefineBasic.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineMultiple) {
  const std::string file_name = "Test02_DefineMultiple.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineRedefine) {
  const std::string file_name = "Test03_DefineRedefine.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineInvalid1) {
  const std::string file_name = "Test04_DefineInvalid1.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineInvalid2) {
  const std::string file_name = "Test05_DefineInvalid2.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineInvalid3) {
  const std::string file_name = "Test06_DefineInvalid3.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineInvalid4) {
  const std::string file_name = "Test07_DefineInvalid4.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineComplexName) {
  const std::string file_name = "Test08_DefineComplexName.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineSpacing) {
  const std::string file_name = "Test09_DefineSpacing.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, DefineComment) {
  const std::string file_name = "Test10_DefineComment.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Undef tests
TEST(PreprocessorUnitTestSuite, UndefNonexistent) {
  const std::string file_name = "Test01_UndefNonexistent.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefMultiple) {
  const std::string file_name = "Test02_UndefMultiple.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefReundef) {
  const std::string file_name = "Test03_UndefReundef.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefInvalid1) {
  const std::string file_name = "Test04_UndefInvalid1.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefInvalid2) {
  const std::string file_name = "Test05_UndefInvalid2.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefInvalid3) {
  const std::string file_name = "Test06_UndefInvalid3.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefInvalid4) {
  const std::string file_name = "Test07_UndefInvalid4.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefComplexName) {
  const std::string file_name = "Test08_UndefComplexName.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefSpacing) {
  const std::string file_name = "Test09_UndefSpacing.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefComment) {
  const std::string file_name = "Test10_UndefComment.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, UndefOrder) {
  const std::string file_name = "Test11_UndefOrder.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Ifdef tests
TEST(PreprocessorUnitTestSuite, IfdefBasic) {
  const std::string file_name = "Test01_IfdefBasic.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefFalse) {
  const std::string file_name = "Test02_IfdefFalse.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefNested) {
  const std::string file_name = "Test03_IfdefNested.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefComplex) {
  const std::string file_name = "Test04_IfdefComplex.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefEmpty) {
  const std::string file_name = "Test05_IfdefEmpty.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefMissingEndif) {
  const std::string file_name = "Test06_IfdefMissingEndif.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefInvalid) {
  const std::string file_name = "Test07_IfdefInvalid.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefOrder) {
  const std::string file_name = "Test08_IfdefOrder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefUndef) {
  const std::string file_name = "Test09_IfdefUndef.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefUndefOrder) {
  const std::string file_name = "Test10_IfdefUndefOrder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefDefineAfter) {
  const std::string file_name = "Test11_IfdefDefineAfter.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefMultiple) {
  const std::string file_name = "Test12_IfdefMultiple.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Ifndef tests
TEST(PreprocessorUnitTestSuite, IfndefBasic) {
  const std::string file_name = "Test01_IfndefBasic.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefFalse) {
  const std::string file_name = "Test02_IfndefFalse.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefNested) {
  const std::string file_name = "Test03_IfndefNested.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefComplex) {
  const std::string file_name = "Test04_IfndefComplex.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefEmpty) {
  const std::string file_name = "Test05_IfndefEmpty.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefMissingendif) {
  const std::string file_name = "Test06_IfndefMissingEndif.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefInvalid) {
  const std::string file_name = "Test07_IfndefInvalid.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefOrder) {
  const std::string file_name = "Test08_IfndefOrder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefUndef) {
  const std::string file_name = "Test09_IfndefUndef.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefUndefOrder) {
  const std::string file_name = "Test10_IfndefUndefOrder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefDefineAfter) {
  const std::string file_name = "Test11_IfndefDefineAfter.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefMultiple) {
  const std::string file_name = "Test12_IfndefMultiple.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Else-extra tests
TEST(PreprocessorUnitTestSuite, IfdefElseBasic) {
  const std::string file_name = "Test01_IfdefElseBasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseFalse) {
  const std::string file_name = "Test02_IfdefElseFalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefElseBasic) {
  const std::string file_name = "Test03_IfndefElseBasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefElseFalse) {
  const std::string file_name = "Test04_IfndefElseFalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseNested) {
  const std::string file_name = "Test05_IfdefElseNested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseComplex) {
  const std::string file_name = "Test06_IfdefElseComplex.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseMissingEndif) {
  const std::string file_name = "Test07_IfdefElseMissingEndif.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseEmpty) {
  const std::string file_name = "Test08_IfdefElseEmpty.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseOrder) {
  const std::string file_name = "Test09_IfdefElseOrder.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseUndef) {
  const std::string file_name = "Test10_IfdefElseUndef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefMultipileElse) {
  const std::string file_name = "Test11_IfdefMultipileElse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseInNested) {
  const std::string file_name = "Test12_IfdefElseInNested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfndefElseInNested) {
  const std::string file_name = "Test13_IfndefElseInNested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseWithRedef) {
  const std::string file_name = "Test14_IfdefElseWithRedef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, IfdefElseEdge) {
  const std::string file_name = "Test15_IfdefElseEdge.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Import tests
TEST(PreprocessorUnitTestSuite, ImportBasic) {
  const std::string file_name = "Test01_ImportBasic.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportMultiple) {
  const std::string file_name = "Test02_ImportMultiple.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportFromSubdirectory) {
  const std::string file_name = "Test03_ImportFromSubdirectory.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportNested) {
  const std::string file_name = "Test04_ImportNested.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportNonexistent) {
  const std::string file_name = "Test05_ImportNonexistent.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportWithDefines) {
  const std::string file_name = "Test06_ImportWithDefines.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportCircular) {
  const std::string file_name = "Test07_ImportCircular.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportUndef) {
  const std::string file_name = "Test08_ImportUndef.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportComplex) {
  const std::string file_name = "Test09_ImportComplex.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ImportInvalid) {
  const std::string file_name = "Test10_ImportInvalid.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Complex tests
TEST(PreprocessorUnitTestSuite, Complex1) {
  const std::string file_name = "Test01_Complex1.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, Complex2) {
  const std::string file_name = "Test02_Complex2.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path input_src = GetInputFilePath(dir_name, file_name);
  const std::filesystem::path expected_src = GetExpectedFilePath(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

} // namespace ovum::compiler::preprocessor
