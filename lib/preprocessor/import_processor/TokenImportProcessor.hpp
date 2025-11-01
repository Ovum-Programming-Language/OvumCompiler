#ifndef TOKENIMPORTPROCESSOR_HPP_
#define TOKENIMPORTPROCESSOR_HPP_

#include <expected>
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "FileGraph.hpp"
#include "lib/lexer/Lexer.hpp"
#include "lib/preprocessor/PreprocessingParameters.hpp"
#include "lib/preprocessor/PreprocessorError.hpp"
#include "lib/preprocessor/TokenProcessor.hpp"
#include "lib/lexer/tokens/Token.hpp"

class TokenImportProcessor : public TokenProcessor {
public:
  TokenImportProcessor(const std::filesystem::path& main_file, const std::set<std::filesystem::path>& include_paths);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) override;

  [[nodiscard]] const std::unordered_map<std::filesystem::path, std::vector<TokenPtr>>& GetFileToTokens() const;

  [[nodiscard]] const std::map<std::filesystem::path, std::set<std::filesystem::path>>&
  GetDependencyGraph() const;

private:
  std::filesystem::path main_file_;
  std::set<std::filesystem::path> include_paths_;

  std::unordered_map<std::filesystem::path, std::vector<TokenPtr>> file_to_tokens_;
  FileGraph file_graph_;
  std::unordered_set<std::filesystem::path> visited_;

  [[nodiscard]] std::expected<std::string, PreprocessorError> ReadFileToString(const std::filesystem::path& file);

  [[nodiscard]] std::expected<void, PreprocessorError> GatherDependencies(const std::filesystem::path& file,
                                                                          const std::vector<TokenPtr>& tokens);

  [[nodiscard]] std::vector<TokenPtr> ConcatenateTokens(const std::vector<std::filesystem::path>& order) const;

  [[nodiscard]] std::vector<TokenPtr> RemoveExtraTokens(const std::vector<TokenPtr>& tokens) const;

  [[nodiscard]] std::expected<std::filesystem::path, PreprocessorError> ResolveImportPath(
    size_t token_index, const std::vector<TokenPtr>& tokens);
};

#endif // TOKENIMPORTPROCESSOR_HPP_
