#include <iostream>

#include "lib/compiler_ui/compiler_ui_functions.hpp"

int main(int32_t argc, char** argv) {
  const std::string sample = R"ovum(NaN)ovum";

  return StartCompilerConsoleUI({"ovumc", sample}, std::cout, std::cerr);
}
