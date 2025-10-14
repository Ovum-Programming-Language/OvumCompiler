#include <filesystem>
#include <sstream>

#include <gtest/gtest.h>
#include "lib/compiler_ui/compiler_ui_functions.hpp" // include your library here
#include "test_functions.hpp"
#include "test_suites/ProjectIntegrationTestSuite.hpp"

TEST_F(ProjectIntegrationTestSuite, InitTest) {
  ASSERT_TRUE(std::filesystem::is_directory(kTemporaryDirectoryName));
}

// TODO: Add tests for positive and negative cases

TEST_F(ProjectIntegrationTestSuite, NegativeTest1) {
  std::ostringstream out;
  ASSERT_EQ(StartCompilerConsoleUI(SplitString("test"), out), 1);
}
