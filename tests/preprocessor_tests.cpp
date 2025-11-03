#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include "lib/lexer/Lexer.hpp"
#include "lib/preprocessor/Preprocessor.hpp"
#include "test_suites/PreprocessorUnitTestSuite.hpp"

namespace ovum::compiler::preprocessor {

std::filesystem::path get_input_file_path(const std::string& addition_path, const std::string& filename) {
  return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "inputs" / addition_path / filename;
}

std::filesystem::path get_expected_file_path(const std::string& addition_path, const std::string& filename) {
  return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / "results" / addition_path / filename;
}

// Define tests
TEST(PreprocessorUnitTestSuite, definebasic) {
  const std::string file_name = "Test01_DefineBasic.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, definemultiple) {
  const std::string file_name = "Test02_DefineMultiple.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, defineredefine) {
  const std::string file_name = "Test03_DefineRedefine.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, defineinvalid1) {
  const std::string file_name = "Test04_DefineInvalid1.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, defineinvalid2) {
  const std::string file_name = "Test05_DefineInvalid2.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, defineinvalid3) {
  const std::string file_name = "Test06_DefineInvalid3.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, defineinvalid4) {
  const std::string file_name = "Test07_DefineInvalid4.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, definecomplexname) {
  const std::string file_name = "Test08_DefineComplexName.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, definespacing) {
  const std::string file_name = "Test09_DefineSpacing.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, definecomment) {
  const std::string file_name = "Test10_DefineComment.txt";
  const std::string dir_name = "define";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Undef tests
TEST(PreprocessorUnitTestSuite, undefnonexistent) {
  const std::string file_name = "Test01_UndefNonexistent.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefmultiple) {
  const std::string file_name = "Test02_UndefMultiple.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefreundef) {
  const std::string file_name = "Test03_UndefReundef.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefinvalid1) {
  const std::string file_name = "Test04_UndefInvalid1.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefinvalid2) {
  const std::string file_name = "Test05_UndefInvalid2.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefinvalid3) {
  const std::string file_name = "Test06_UndefInvalid3.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefinvalid4) {
  const std::string file_name = "Test07_UndefInvalid4.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefcomplexname) {
  const std::string file_name = "Test08_UndefComplexName.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefspacing) {
  const std::string file_name = "Test09_UndefSpacing.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undefcomment) {
  const std::string file_name = "Test10_UndefComment.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, undeforder) {
  const std::string file_name = "Test11_UndefOrder.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Ifdef tests
TEST(PreprocessorUnitTestSuite, ifdefbasic) {
  const std::string file_name = "Test01_IfdefBasic.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdeffalse) {
  const std::string file_name = "Test02_IfdefFalse.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefnested) {
  const std::string file_name = "Test03_IfdefNested.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefcomplex) {
  const std::string file_name = "Test04_IfdefComplex.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefempty) {
  const std::string file_name = "Test05_IfdefEmpty.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefmissingendif) {
  const std::string file_name = "Test06_IfdefMissingEndif.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefinvalid) {
  const std::string file_name = "Test07_IfdefInvalid.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdeforder) {
  const std::string file_name = "Test08_IfdefOrder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefundef) {
  const std::string file_name = "Test09_IfdefUndef.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefundeforder) {
  const std::string file_name = "Test10_IfdefUndefOrder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefdefineafter) {
  const std::string file_name = "Test11_IfdefDefineAfter.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefmultiple) {
  const std::string file_name = "Test12_IfdefMultiple.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Ifndef tests
TEST(PreprocessorUnitTestSuite, ifndefbasic) {
  const std::string file_name = "Test01_IfndefBasic.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndeffalse) {
  const std::string file_name = "Test02_IfndefFalse.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefnested) {
  const std::string file_name = "Test03_IfndefNested.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefcomplex) {
  const std::string file_name = "Test04_IfndefComplex.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefempty) {
  const std::string file_name = "Test05_IfndefEmpty.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefmissingendif) {
  const std::string file_name = "Test06_IfndefMissingEndif.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefinvalid) {
  const std::string file_name = "Test07_IfndefInvalid.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndeforder) {
  const std::string file_name = "Test08_IfndefOrder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefundef) {
  const std::string file_name = "Test09_IfndefUndef.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefundeforder) {
  const std::string file_name = "Test10_IfndefUndefOrder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefdefineafter) {
  const std::string file_name = "Test11_IfndefDefineAfter.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefmultiple) {
  const std::string file_name = "Test12_IfndefMultiple.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Else-extra tests
TEST(PreprocessorUnitTestSuite, ifdefelsebasic) {
  const std::string file_name = "Test01_IfdefElseBasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelsefalse) {
  const std::string file_name = "Test02_IfdefElseFalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefelsebasic) {
  const std::string file_name = "Test03_IfndefElseBasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefelsefalse) {
  const std::string file_name = "Test04_IfndefElseFalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelsenested) {
  const std::string file_name = "Test05_IfdefElseNested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelsecomplex) {
  const std::string file_name = "Test06_IfdefElseComplex.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelsemissingendif) {
  const std::string file_name = "Test07_IfdefElseMissingEndif.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelseempty) {
  const std::string file_name = "Test08_IfdefElseEmpty.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelseorder) {
  const std::string file_name = "Test09_IfdefElseOrder.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelseundef) {
  const std::string file_name = "Test10_IfdefElseUndef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefmultipileelse) {
  const std::string file_name = "Test11_IfdefMultipileElse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelseinnested) {
  const std::string file_name = "Test12_IfdefElseInnested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifndefelseinnested) {
  const std::string file_name = "Test13_IfndefElseInnested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelsewithredef) {
  const std::string file_name = "Test14_IfdefElseWithRedef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, ifdefelseedge) {
  const std::string file_name = "Test15_IfdefElseEdge.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Import tests
TEST(PreprocessorUnitTestSuite, importbasic) {
  const std::string file_name = "Test01_ImportBasic.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importmultiple) {
  const std::string file_name = "Test02_ImportMultiple.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importfromsubdirectory) {
  const std::string file_name = "Test03_ImportFromSubdirectory.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importnested) {
  const std::string file_name = "Test04_ImportNested.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importnonexistent) {
  const std::string file_name = "Test05_ImportNonexistent.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importwithdefines) {
  const std::string file_name = "Test06_ImportWithDefines.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importcircular) {
  const std::string file_name = "Test07_ImportCircular.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importundef) {
  const std::string file_name = "Test08_ImportUndef.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importcomplex) {
  const std::string file_name = "Test09_ImportComplex.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, importinvalid) {
  const std::string file_name = "Test10_ImportInvalid.txt";
  const std::string dir_name = "import";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

// Complex tests
TEST(PreprocessorUnitTestSuite, complex1) {
  const std::string file_name = "Test01_Complex1.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

TEST(PreprocessorUnitTestSuite, complex2) {
  const std::string file_name = "Test02_Complex2.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path input_src = get_input_file_path(dir_name, file_name);
  const std::filesystem::path expected_src = get_expected_file_path(dir_name, file_name);

  PreprocessorUnitTestSuite::RunSingleTest(input_src, expected_src);
}

} // namespace ovum::compiler::preprocessor
