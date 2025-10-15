#include "lib/lexer/Lexer.hpp"

#include "compiler_ui_functions.hpp"

int32_t StartCompilerConsoleUI(const std::vector<std::string>& args, std::ostream& out) {
  if (args.size() < 2) {
    out << "Insufficient arguments\n";
    return 1;
  }

  const std::string& sample = args[1];
  Lexer lx(sample, false);

  try {
    auto toks = lx.Tokenize();

    for (auto& t : toks) {
      out << t->ToString() << "\n";
    }
  } catch (const std::exception& e) {
    out << "Lexer error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
