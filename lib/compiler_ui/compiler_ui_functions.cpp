#include "lib/lexer/Lexer.hpp"
#include "lib/preprocessor/Preprocessor.hpp"

#include <exception>

#include "compiler_ui_functions.hpp"

int32_t StartCompilerConsoleUI(const std::vector<std::string>& args, std::ostream& out, std::ostream& err) {
  if (args.size() < 2) {
    err << "Usage: ovumc <main_file.ovum> [include_path1] [include_path2] ...\n";
    err << "Example: ovumc sample.ovum /path/to/includes\n";
    return 1;
  }

  if (args[1] == "--help") {
    out << "Usage: ovumc <main_file.ovum> [include_path1] [include_path2] ...\n";
    out << "Example: ovumc sample.ovum /path/to/includes\n";
    return 0;
  }

  std::filesystem::path main_file = args[1];

  std::set<std::filesystem::path> include_paths;
  for (size_t i = 2; i < args.size(); ++i) {
    include_paths.emplace(args[i]);
  }

  std::unordered_set<std::string> predefined_symbols;

  PreprocessingParameters params{.include_paths = std::move(include_paths),
                                 .predefined_symbols = std::move(predefined_symbols),
                                 .main_file = std::move(main_file)};

  Preprocessor preprocessor(std::move(params));

  auto result = preprocessor.Process();
  if (!result) {
    err << result.error().what() << "\n";
    return 1;
  }

  const auto& tokens = result.value();
  for (const auto& t : tokens) {
    out << t->ToString() << "\n";
  }

  out << "\nPreprocessed " << tokens.size() << " tokens successfully.\n";
  return 0;
}
