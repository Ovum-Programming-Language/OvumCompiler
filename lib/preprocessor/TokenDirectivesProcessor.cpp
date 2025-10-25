#include "TokenDirectivesProcessor.hpp"

#include <sstream>

TokenDirectivesProcessor::TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols) {
  for (const auto& sym : predefined_symbols) {
    macros_[sym] = "({})";
  }
}

std::expected<std::vector<TokenPtr>, PreprocessorError> TokenDirectivesProcessor::Process(
    const std::vector<TokenPtr>& tokens) {
  std::vector<TokenPtr> result;
  size_t i = 0;
  bool skipping = false;
  int skip_level = 0;

  while (i < tokens.size()) {
    const auto& t = tokens[i];
    std::string type = t->GetStringType();

    if (type == "#define") {
      if (i + 2 >= tokens.size()) {
        return std::unexpected(
            InvalidImportError("Incomplete #define at line " + std::to_string(t->GetPosition().GetLine())));
      }
      const auto& id_token = tokens[i + 1];
      const auto& value_token = tokens[i + 2];
      if (!is_identifier(id_token)) {
        return std::unexpected(InvalidImportError("Expected identifier after #define at line " +
                                                  std::to_string(id_token->GetPosition().GetLine())));
      }
      std::string id = id_token->GetLexeme();
      std::string value = value_token->GetLexeme();

      if (!skipping) {
        macros_[id] = value;
      }
      i += 3;
      continue;
    }

    if (type == "#undef") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(
            InvalidImportError("Incomplete #undef at line " + std::to_string(t->GetPosition().GetLine())));
      }
      const auto& id_token = tokens[i + 1];
      if (!is_identifier(id_token)) {
        return std::unexpected(InvalidImportError("Expected identifier after #undef at line " +
                                                  std::to_string(id_token->GetPosition().GetLine())));
      }
      std::string id = id_token->GetLexeme();

      if (!skipping) {
        macros_.erase(id);
      }
      i += 2;
      continue;
    }

    if (type == "#ifdef" || type == "#ifndef") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(InvalidImportError("Incomplete #" + type.substr(1) + " at line " +
                                                  std::to_string(t->GetPosition().GetLine())));
      }
      const auto& id_token = tokens[i + 1];
      if (!is_identifier(id_token)) {
        return std::unexpected(InvalidImportError("Expected identifier after #" + type.substr(1) + " at line " +
                                                  std::to_string(id_token->GetPosition().GetLine())));
      }
      std::string id = id_token->GetLexeme();

      bool cond = macros_.count(id);
      if (type == "#ifndef") {
        cond = !cond;
      }

      if (skipping) {
        skip_level++;
      } else {
        if (!cond) {
          skipping = true;
          skip_level = 1;
        }
      }
      i += 2;
      continue;
    }

    if (type == "#else") {
      if (skip_level == 0) {
        return std::unexpected(
            InvalidImportError("Mismatched #else at line " + std::to_string(t->GetPosition().GetLine())));
      }
      if (skip_level == 1 && !skipping) {
        skipping = true;
      }
      i += 1;
      continue;
    }

    if (type == "#endif") {
      if (skip_level == 0) {
        return std::unexpected(
            InvalidImportError("Mismatched #endif at line " + std::to_string(t->GetPosition().GetLine())));
      }
      skip_level--;
      if (skip_level == 0) {
        skipping = false;
      }
      i += 1;
      continue;
    }

    if (!skipping) {
      result.push_back(t);
    }
    ++i;
  }

  if (skip_level > 0) {
    return std::unexpected(InvalidImportError("Unmatched #if directive"));
  }

  return {std::move(result)};
}

bool TokenDirectivesProcessor::is_identifier(const TokenPtr& token) const {
  return token->GetStringType() == "IDENT";
}
