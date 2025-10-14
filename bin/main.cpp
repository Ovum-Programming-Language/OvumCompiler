#include <iostream>

#include "lib/compiler_ui/compiler_ui_functions.hpp"

int main() {
  const std::string sample = R"ovum(
// demo
fun Main(args: StringArray): Int {
    val count: Int = args.Length()
    sys::Print("Args count: " + count.ToString())
    return 0
}
)ovum";

  return StartCompilerConsoleUI({"ovumc", sample}, std::cout);
}
