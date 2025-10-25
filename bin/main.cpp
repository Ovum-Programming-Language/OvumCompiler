#include <iostream>

#include "lib/compiler_ui/compiler_ui_functions.hpp"

int main(int32_t argc, char** argv) {
  std::vector<std::string> args = std::vector<std::string>(argv, argv + argc);
  return StartCompilerConsoleUI(args, std::cout, std::cerr);
}

