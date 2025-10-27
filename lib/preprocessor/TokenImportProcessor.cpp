#include <algorithm>
#include <filesystem>
#include <fstream>
#include <queue>
#include <string_view>

#include "TokenImportProcessor.hpp"

const std::unordered_map<std::filesystem::path, std::unordered_set<std::filesystem::path>>&
TokenImportProcessor::GetDepGraph() const {
  return dep_graph_;
}

const std::unordered_map<std::filesystem::path, std::vector<TokenPtr>>& TokenImportProcessor::GetFileToTokens() const {
  return file_to_tokens_;
}

std::expected<std::string, PreprocessorError> TokenImportProcessor::ReadFileToString(
    const std::filesystem::path& file) {
  if (!std::filesystem::exists(file)) {
    return std::unexpected(FileNotFoundError(file.string()));
  }

  std::ifstream file_stream(file, std::ios::binary);

  if (!file_stream.is_open()) {
    return std::unexpected(FileReadError(file.string(), "Cannot open"));
  }

  std::string content((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());

  if (file_stream.bad() || file_stream.fail()) {
    return std::unexpected(FileReadError(file.string(), "Read error"));
  }

  return {std::move(content)};
}

TokenImportProcessor::TokenImportProcessor(std::filesystem::path main_file,
                                           std::set<std::filesystem::path> include_paths) :
    main_file_(main_file), include_paths_(std::move(include_paths)) {
}

std::expected<std::vector<TokenPtr>, PreprocessorError> TokenImportProcessor::Process(
    const std::vector<TokenPtr>& tokens) {
  file_to_tokens_.clear();
  dep_graph_.clear();
  visited_.clear();

  std::expected<void, PreprocessorError> dep_result = GatherDependencies(main_file_, tokens);

  if (!dep_result) {
    return std::unexpected(dep_result.error());
  }

  std::unordered_map<std::filesystem::path, int> colors;
  std::vector<std::filesystem::path> cycle_path;

  for (const auto& [node, _] : file_to_tokens_) {
    if (colors.find(node) == colors.end()) {
      if (DetectCycles(node, colors, cycle_path)) {
        std::string cycle_str;
        for (const std::filesystem::path& p : cycle_path) {
          cycle_str += p.string() + " -> ";
        }
        cycle_str += cycle_path.front().string();
        return std::unexpected(CycleDetectedError("Cycle detected: " + cycle_str));
      }
    }
  }

  std::expected<std::vector<std::filesystem::path>, PreprocessorError> order_result = TopologicalSort();

  if (!order_result) {
    return std::unexpected(order_result.error());
  }

  std::vector<std::filesystem::path> order = order_result.value();

  std::vector<TokenPtr> concatenated = ConcatenateTokens(order);

  std::vector<TokenPtr> cleaned = RemoveImports(concatenated);

  cleaned = RemoveExtraEofs(cleaned);

  return {std::move(cleaned)};
}

std::expected<void, PreprocessorError> TokenImportProcessor::GatherDependencies(const std::filesystem::path& file,
                                                                                const std::vector<TokenPtr>& tokens) {
  if (visited_.count(file) != 0) {
    return {};
  }

  visited_.insert(file);
  file_to_tokens_[file] = tokens;

  size_t i = 0;

  while (i < tokens.size()) {
    const TokenPtr& token = tokens[i];
    if (token->GetLexeme() == "#import") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(InvalidImportError("Missing path after #import at " +
                                                  std::to_string(token->GetPosition().GetLine()) + ":" +
                                                  std::to_string(token->GetPosition().GetColumn())));
      }

      const TokenPtr& path_token = tokens[i + 1];

      if (path_token->GetStringType() != "LITERAL:String") {
        return std::unexpected(InvalidImportError("Expected string literal after #import at " +
                                                  std::to_string(token->GetPosition().GetLine()) + ":" +
                                                  std::to_string(token->GetPosition().GetColumn())));
      }

      const std::string& import_lexeme = path_token->GetLexeme();

      std::expected<std::filesystem::path, PreprocessorError> dep_path_result =
          ResolveImportPath(import_lexeme);

      if (!dep_path_result) {
        return std::unexpected(FileNotFoundError(import_lexeme));
      }

      const std::filesystem::path& dep_path = dep_path_result.value();

      dep_graph_[file].insert(dep_path);

      if (file_to_tokens_.count(dep_path) == 0) {
        std::expected<std::string, PreprocessorError> content_result = ReadFileToString(dep_path);

        if (!content_result) {
          return std::unexpected(content_result.error());
        }

        std::string content_str = std::move(content_result.value());
        std::string_view content_view(content_str);

        Lexer lexer(content_view, false);
        std::vector<TokenPtr> raw_tokens;

        try {
          raw_tokens = lexer.Tokenize();
        } catch (const std::exception& e) {
          return std::unexpected(PreprocessorError("Lexer error for " + dep_path.string() + ": " + e.what()));
        }

        std::expected<void, PreprocessorError> sub_result = GatherDependencies(dep_path, raw_tokens);

        if (!sub_result) {
          return sub_result;
        }
      }
      i += 2;
      continue;
    }
    ++i;
  }

  return {};
}

bool TokenImportProcessor::DetectCycles(const std::filesystem::path& node,
                                        std::unordered_map<std::filesystem::path, int>& colors,
                                        std::vector<std::filesystem::path>& cycle_path) const {
  colors[node] = 1;
  cycle_path.push_back(node);

  auto it = dep_graph_.find(node);

  if (it != dep_graph_.end()) {
    for (const auto& neighbor : it->second) {
      auto color_it = colors.find(neighbor);
      if (color_it == colors.end()) {
        if (DetectCycles(neighbor, colors, cycle_path)) {
          return true;
        }
      } else if (color_it->second == 1) {
        return true;
      }
    }
  }

  colors[node] = 2;
  cycle_path.pop_back();

  return false;
}

std::expected<std::vector<std::filesystem::path>, PreprocessorError> TokenImportProcessor::TopologicalSort() const {
  if (file_to_tokens_.empty()) {
    return {};
  }

  std::unordered_map<std::filesystem::path, int> in_degree;

  for (const auto& [u, _] : file_to_tokens_) {
    in_degree[u] = 0;
  }

  for (const auto& [u, deps] : dep_graph_) {
    for (const std::filesystem::path& v : deps) {
      auto it = in_degree.find(v);
      if (it != in_degree.end()) {
        ++(it->second);
      } else {
        in_degree[v] = 1;
      }
    }
  }

  std::queue<std::filesystem::path> q;

  for (const auto& [node, deg] : in_degree) {
    if (deg == 0) {
      q.push(node);
    }
  }

  std::vector<std::filesystem::path> order;

  while (!q.empty()) {
    std::filesystem::path u = q.front();
    q.pop();
    order.push_back(u);

    auto deps_it = dep_graph_.find(u);

    if (deps_it != dep_graph_.end()) {
      for (const std::filesystem::path& v : deps_it->second) {
        auto deg_it = in_degree.find(v);

        if (deg_it != in_degree.end()) {
          --(deg_it->second);

          if (deg_it->second == 0) {
            q.push(v);
          }
        }
      }
    }
  }

  if (order.size() != file_to_tokens_.size()) {
    return std::unexpected(CycleDetectedError("Topological sort failed (cycle)"));
  }

  std::ranges::reverse(order);

  return order;
}

std::vector<TokenPtr> TokenImportProcessor::ConcatenateTokens(const std::vector<std::filesystem::path>& order) const {
  std::vector<TokenPtr> total;
  for (const std::filesystem::path& path : order) {
    auto it = file_to_tokens_.find(path);

    if (it != file_to_tokens_.end()) {
      total.insert(total.end(), it->second.begin(), it->second.end());
    }
  }

  return total;
}

std::vector<TokenPtr> TokenImportProcessor::RemoveImports(const std::vector<TokenPtr>& tokens) const {
  std::vector<TokenPtr> cleaned;
  size_t i = 0;
  while (i < tokens.size()) {
    const TokenPtr& token = tokens[i];

    if (token->GetLexeme() == "#import") {
      if (i + 1 < tokens.size() && tokens[i + 1]->GetStringType() == "LITERAL:String") {
        i += 2;
      } else {
        cleaned.push_back(token);
        ++i;
      }
    } else {
      cleaned.push_back(token);
      ++i;
    }
  }

  return cleaned;
}

std::vector<TokenPtr> TokenImportProcessor::RemoveExtraEofs(const std::vector<TokenPtr>& tokens) const {
  if (tokens.empty()) {
    return {};
  }

  std::vector<TokenPtr> cleaned;
  cleaned.reserve(tokens.size());

  for (size_t i = 0; i < tokens.size(); ++i) {
    const TokenPtr& t = tokens[i];

    if (t->GetStringType() != "EOF") {
      cleaned.push_back(t);
    } else if (i == tokens.size() - 1) {
      cleaned.push_back(t);
    }
  }

  return cleaned;
}

std::expected<std::filesystem::path, PreprocessorError> TokenImportProcessor::ResolveImportPath(
    const std::string& import_lexeme) {
  if (import_lexeme.size() < 2 || import_lexeme[0] != '"' || import_lexeme.back() != import_lexeme[0]) {
    return std::unexpected(InvalidImportError("Invalid quote in " + import_lexeme));
  }

  std::string name = import_lexeme.substr(1, import_lexeme.size() - 2);

  std::filesystem::path candidate;

  for (const std::filesystem::path& inc : include_paths_) {
    candidate = inc / name;

    if (std::filesystem::exists(candidate)) {
      return candidate;
    }
  }

  return std::unexpected(PreprocessorError("Import not found: " + name));
}
