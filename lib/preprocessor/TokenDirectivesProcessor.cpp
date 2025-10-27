#include "TokenDirectivesProcessor.hpp"

#include <sstream>

TokenDirectivesProcessor::TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols) {
  for (const std::string& sym : predefined_symbols) {
    macros_[sym];
  }
}

std::expected<std::vector<TokenPtr>, PreprocessorError> TokenDirectivesProcessor::Process(
    const std::vector<TokenPtr>& tokens) {
  std::vector<TokenPtr> result;

  size_t i = 0;

  bool skipping = false;
  int skip_level = 0;
  int if_level = 0;

  while (i < tokens.size()) {
    const TokenPtr& t = tokens[i];
    std::string lexeme = t->GetLexeme();

    if (lexeme == "#define") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(
            InvalidImportError("Incomplete #define at line " + std::to_string(t->GetPosition().GetLine())));
      }

      const TokenPtr& id_token = tokens[i + 1];

      if (!(id_token->GetStringType() == "IDENT")) {
        return std::unexpected(InvalidImportError("Expected identifier after #define at line " +
                                                  std::to_string(id_token->GetPosition().GetLine())));
      }

      std::string id = id_token->GetLexeme();

      size_t start = i + 2;

      if (start >= tokens.size()) {
        return std::unexpected(InvalidImportError("No value after #define " + id + " at line " +
                                                  std::to_string(t->GetPosition().GetLine())));
      }

      size_t end = start;

      while (end < tokens.size() && tokens[end]->GetStringType() != "NEWLINE") {
        ++end;
      }

      std::vector<TokenPtr> expansion;

      for (size_t k = start; k < end; ++k) {
        expansion.push_back(tokens[k]);
      }

      if (!skipping) {
        macros_[id] = std::move(expansion);
      }

      i = end + 1;
      continue;
    }

    if (lexeme == "#undef") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(
            InvalidImportError("Incomplete #undef at line " + std::to_string(t->GetPosition().GetLine())));
      }

      const TokenPtr& id_token = tokens[i + 1];

      if (!(id_token->GetStringType() == "IDENT")) {
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

    if (lexeme == "#ifdef" || lexeme == "#ifndef") {
      if (i + 1 >= tokens.size()) {
        return std::unexpected(InvalidImportError("Incomplete #" + lexeme.substr(1) + " at line " +
                                                  std::to_string(t->GetPosition().GetLine())));
      }

      const TokenPtr& id_token = tokens[i + 1];

      if (!(id_token->GetStringType() == "IDENT")) {
        return std::unexpected(InvalidImportError("Expected identifier after #" + lexeme.substr(1) + " at line " +
                                                  std::to_string(id_token->GetPosition().GetLine())));
      }

      std::string id = id_token->GetLexeme();

      bool cond = macros_.count(id) > 0;

      if (lexeme == "#ifndef") {
        cond = !cond;
      }

      if_level++;

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

    if (lexeme == "#else") {
      if (if_level == 0) {
        return std::unexpected(
            InvalidImportError("Mismatched #else at line " + std::to_string(t->GetPosition().GetLine())));
      }

      if (!skipping) {
        skipping = true;
      } else if (skip_level == 1) {
        skipping = false;
      }

      i += 1;
      continue;
    }

    if (lexeme == "#endif") {
      if (if_level == 0) {
        return std::unexpected(
            InvalidImportError("Mismatched #endif at line " + std::to_string(t->GetPosition().GetLine())));
      }

      if_level--;

      if (skip_level > 0) {
        skip_level--;
      }

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

  if (skip_level > 0 || if_level > 0) {
    return std::unexpected(InvalidImportError("Unmatched #if directive"));
  }

  std::vector<TokenPtr> expanded;

  for (const TokenPtr& tok : result) {
    if (tok->GetStringType() == "IDENT" && macros_.count(tok->GetLexeme()) > 0) {
      const std::vector<TokenPtr>& exp_tokens = macros_[tok->GetLexeme()];
      expanded.insert(expanded.end(), exp_tokens.begin(), exp_tokens.end());
    } else {
      expanded.push_back(tok);
    }
  }

  return {std::move(expanded)};
}
