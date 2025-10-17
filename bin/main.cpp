#include <iostream>

#include "lib/compiler_ui/compiler_ui_functions.hpp"
#include "lib/lexer/Lexer.hpp"

int main(int32_t argc, char** argv) {
  const std::string sample = R"ovum('' '')ovum";

  Lexer lexer("0 3.14 0.5 5.00");

  auto tokens = lexer.Tokenize();
  for (auto& t : tokens) {
    std::cout << t->ToString() << "\n";
  }

  std::cout << "\n\n";

  return StartCompilerConsoleUI({"ovumc", sample}, std::cout, std::cerr);
}
