#include "lib/lexer/Lexer.hpp"

#include "compiler_ui_functions.hpp"

int32_t StartCompilerConsoleUI(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  if (args.size() < 2) {
    err << "Insufficient arguments\n";
    return 1;
  }

  const std::string& sample = args[1];
  Lexer lx(sample, true);

  try {
    auto toks = lx.Tokenize();

    for (auto& t : toks) {
      out << t->ToString() << "\n";
    }
  } catch (const std::exception& e) {
    err << "Lexer error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
