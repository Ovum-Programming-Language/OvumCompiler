#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include "lib/lexer/Lexer.hpp"
#include "test_suites/PreprocessorUnitTestSuite.hpp"

std::filesystem::path get_test_file_path(const std::string& addition_path, const std::string& filename) {
    return std::filesystem::path(TEST_DATA_DIR) / "preprocessor" / addition_path / filename;
}

// Define tests
TEST(PreprocessorUnitTestSuite, definebasic) {
  const std::string file_name = "test01definebasic.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, definemultiple) {
  const std::string file_name = "test02definemultiple.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, defineredefine) {
  const std::string file_name = "test03defineredefine.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, defineinvalid1) {
  const std::string file_name = "test04defineinvalid1.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, defineinvalid2) {
  const std::string file_name = "test05defineinvalid2.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, defineinvalid3) {
  const std::string file_name = "test06defineinvalid3.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, defineinvalid4) {
  const std::string file_name = "test07defineinvalid4.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, definecomplexname) {
  const std::string file_name = "test08definecomplexname.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, definespacing) {
  const std::string file_name = "test09definespacing.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, definecomment) {
  const std::string file_name = "test10definecomment.txt";
  const std::string dir_name = "define";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Undef tests
TEST(PreprocessorUnitTestSuite, undefnonexistent) {
  const std::string file_name = "test01undefnonexistent.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefmultiple) {
  const std::string file_name = "test02undefmultiple.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefreundef) {
  const std::string file_name = "test03undefreundef.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefinvalid1) {
  const std::string file_name = "test04undefinvalid1.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefinvalid2) {
  const std::string file_name = "test05undefinvalid2.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefinvalid3) {
  const std::string file_name = "test06undefinvalid3.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefinvalid4) {
  const std::string file_name = "test07undefinvalid4.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefcomplexname) {
  const std::string file_name = "test08undefcomplexname.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefspacing) {
  const std::string file_name = "test09undefspacing.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undefcomment) {
  const std::string file_name = "test10undefcomment.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, undeforder) {
  const std::string file_name = "test11undeforder.txt";
  const std::string dir_name = "undef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Ifdef tests
TEST(PreprocessorUnitTestSuite, ifdefbasic) {
  const std::string file_name = "test01ifdefbasic.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdeffalse) {
  const std::string file_name = "test02ifdeffalse.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefnested) {
  const std::string file_name = "test03ifdefnested.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefcomplex) {
  const std::string file_name = "test04ifdefcomplex.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefempty) {
  const std::string file_name = "test05ifdefempty.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefmissingendif) {
  const std::string file_name = "test06ifdefmissingendif.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefinvalid) {
  const std::string file_name = "test07ifdefinvalid.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdeforder) {
  const std::string file_name = "test08ifdeforder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefundef) {
  const std::string file_name = "test09ifdefundef.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefundeforder) {
  const std::string file_name = "test10ifdefundeforder.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefdefineafter) {
  const std::string file_name = "test11ifdefdefineafter.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefmultiple) {
  const std::string file_name = "test12ifdefmultiple.txt";
  const std::string dir_name = "ifdef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Ifndef tests
TEST(PreprocessorUnitTestSuite, ifndefbasic) {
  const std::string file_name = "test01ifndefbasic.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndeffalse) {
  const std::string file_name = "test02ifndeffalse.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefnested) {
  const std::string file_name = "test03ifndefnested.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefcomplex) {
  const std::string file_name = "test04ifndefcomplex.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefempty) {
  const std::string file_name = "test05ifndefempty.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefmissingendif) {
  const std::string file_name = "test06ifndefmissingendif.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefinvalid) {
  const std::string file_name = "test07ifndefinvalid.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndeforder) {
  const std::string file_name = "test08ifndeforder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefundef) {
  const std::string file_name = "test09ifndefundef.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefundeforder) {
  const std::string file_name = "test10ifndefundeforder.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefdefineafter) {
  const std::string file_name = "test11ifndefdefineafter.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefmultiple) {
  const std::string file_name = "test12ifndefmultiple.txt";
  const std::string dir_name = "ifndef";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Else-extra tests
TEST(PreprocessorUnitTestSuite, ifdefelsebasic) {
  const std::string file_name = "test01ifdefelsebasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelsefalse) {
  const std::string file_name = "test02ifdefelsefalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefelsebasic) {
  const std::string file_name = "test03ifndefelsebasic.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefelsefalse) {
  const std::string file_name = "test04ifndefelsefalse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelsenested) {
  const std::string file_name = "test05ifdefelsenested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelsecomplex) {
  const std::string file_name = "test06ifdefelsecomplex.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelsemissingendif) {
  const std::string file_name = "test07ifdefelsemissingendif.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelseempty) {
  const std::string file_name = "test08ifdefelseempty.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelseorder) {
  const std::string file_name = "test09ifdefelseorder.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelseundef) {
  const std::string file_name = "test10ifdefelseundef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefmultipileelse) {
  const std::string file_name = "test11ifdefmultipileelse.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelseinnested) {
  const std::string file_name = "test12ifdefelseinnested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifndefelseinnested) {
  const std::string file_name = "test13ifndefelseinnested.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelsewithredef) {
  const std::string file_name = "test14ifdefelsewithredef.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, ifdefelseedge) {
  const std::string file_name = "test15ifdefelseedge.txt";
  const std::string dir_name = "else-extra";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Import tests
TEST(PreprocessorUnitTestSuite, importbasic) {
  const std::string file_name = "test01importbasic.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importmultiple) {
  const std::string file_name = "test02importmultiple.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importconditional) {
  const std::string file_name = "test03importconditional.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importnested) {
  const std::string file_name = "test04importnested.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importnonexistent) {
  const std::string file_name = "test05importnonexistent.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importwithdefines) {
  const std::string file_name = "test06importwithdefines.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importcircular) {
  const std::string file_name = "test07importcircular.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importundef) {
  const std::string file_name = "test08importundef.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importcomplex) {
  const std::string file_name = "test09importcomplex.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, importinvalid) {
  const std::string file_name = "test10importinvalid.txt";
  const std::string dir_name = "import";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

// Complex tests
TEST(PreprocessorUnitTestSuite, complex1) {
  const std::string file_name = "test01complex1.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}

TEST(PreprocessorUnitTestSuite, complex2) {
  const std::string file_name = "test02complex2.txt";
  const std::string dir_name = "complex";
  const std::filesystem::path file_src = get_test_file_path(dir_name, file_name);
  PreprocessorUnitTestSuite::TestResult result = PreprocessorUnitTestSuite::RunSingleTest(file_src);
  if (!result.passed) {
    std::cout << "Test dir name:     " << dir_name << "\n";
    std::cout << "Test file name:    " << file_name << "\n";
    std::cout << "Error msg:         " << result.error_message << "\n";
  }
  ASSERT_TRUE(result.passed);
}