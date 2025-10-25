#ifndef TOKENIMPORTPROCESSOR_HPP_
#define TOKENIMPORTPROCESSOR_HPP_

#include <algorithm>
#include <expected>
#include <filesystem>
#include <fstream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "PreprocessorError.hpp"
#include "lib/lexer/Lexer.hpp"
#include "lib/lexer/tokens/Token.hpp"

class TokenImportProcessor {
public:
  explicit TokenImportProcessor(PreprocessingParameters parameters);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process();

  [[nodiscard]] const auto& GetFileToTokens() const {
    return file_to_tokens_;
  }
  [[nodiscard]] const auto& GetDepGraph() const {
    return dep_graph_;
  }

private:
  PreprocessingParameters parameters_;

  std::unordered_map<std::filesystem::path, std::vector<TokenPtr>> file_to_tokens_;
  std::unordered_map<std::filesystem::path, std::unordered_set<std::filesystem::path>> dep_graph_;

  std::unordered_set<std::filesystem::path> visited_;

  [[nodiscard]] static std::expected<std::string, PreprocessorError> ReadFileToString(
      const std::filesystem::path& file);

  void gather_dependencies(const std::filesystem::path& file);

  [[nodiscard]] bool detect_cycles(const std::filesystem::path& node,
                                   std::unordered_map<std::filesystem::path, int>& colors,
                                   std::vector<std::filesystem::path>& cycle_path) const;

  [[nodiscard]] std::vector<std::filesystem::path> topological_sort() const;

  [[nodiscard]] std::vector<TokenPtr> concatenate_tokens(const std::vector<std::filesystem::path>& order) const;

  [[nodiscard]] std::vector<TokenPtr> remove_imports(const std::vector<TokenPtr>& tokens) const;

  [[nodiscard]] static std::expected<std::filesystem::path, PreprocessorError> resolve_import_path(
      const std::filesystem::path& current_dir,
      const std::string& import_lexeme,
      const std::set<std::filesystem::path>& include_paths);
};

#endif // TOKENIMPORTPROCESSOR_HPP_
