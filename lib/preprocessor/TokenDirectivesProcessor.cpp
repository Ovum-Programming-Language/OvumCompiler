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
      auto define_result = HandleDefine(i, tokens, skipping);
      if (!define_result) {
        return std::unexpected(define_result.error());
      }
      continue;
    }

    if (lexeme == "#undef") {
      auto undef_result = HandleUndef(i, tokens, skipping);
      if (!undef_result) {
        return std::unexpected(undef_result.error());
      }
      continue;
    }

    if (lexeme == "#ifdef") {
      auto ifdef_result = HandleIfdefOrIfndef(i, tokens, skipping, skip_level, if_level, false);
      if (!ifdef_result) {
        return std::unexpected(ifdef_result.error());
      }
      continue;
    }

    if (lexeme == "#ifndef") {
      auto ifndef_result = HandleIfdefOrIfndef(i, tokens, skipping, skip_level, if_level, true);
      if (!ifndef_result) {
        return std::unexpected(ifndef_result.error());
      }
      continue;
    }

    if (lexeme == "#else") {
      auto else_result = HandleElse(i, tokens, skipping, skip_level, if_level);
      if (!else_result) {
        return std::unexpected(else_result.error());
      }
      continue;
    }

    if (lexeme == "#endif") {
      auto endif_result = HandleEndif(i, tokens, skipping, skip_level, if_level);
      if (!endif_result) {
        return std::unexpected(endif_result.error());
      }
      continue;
    }

    if (!skipping) {
      result.push_back(t);
    }

    ++i;
  }

  if (skip_level > 0 || if_level > 0) {
    return std::unexpected(UnmatchedDirectiveError("Unmatched #if directive"));
  }

  std::vector<TokenPtr> expanded = ExpandMacros(result);

  return {std::move(expanded)};
}

std::expected<void, PreprocessorError> TokenDirectivesProcessor::HandleDefine(size_t& i,
                                                                              const std::vector<TokenPtr>& tokens,
                                                                              bool skipping) {
  if (i + 1 >= tokens.size()) {
    return std::unexpected(
        InvalidDirectiveError("Incomplete #define at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[i + 1];

  if (!(id_token->GetStringType() == "IDENT")) {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #define at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  size_t start = i + 2;

  if (start >= tokens.size()) {
    return std::unexpected(InvalidDirectiveError("No value after #define " + id + " at line " +
                                                 std::to_string(tokens[i]->GetPosition().GetLine())));
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
  return {};
}

std::expected<void, PreprocessorError> TokenDirectivesProcessor::HandleUndef(size_t& i,
                                                                             const std::vector<TokenPtr>& tokens,
                                                                             bool skipping) {
  if (i + 1 >= tokens.size()) {
    return std::unexpected(
        InvalidDirectiveError("Incomplete #undef at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[i + 1];

  if (!(id_token->GetStringType() == "IDENT")) {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #undef at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  if (!skipping) {
    macros_.erase(id);
  }

  i += 2;
  return {};
}

std::expected<void, PreprocessorError> TokenDirectivesProcessor::HandleIfdefOrIfndef(
    size_t& i, const std::vector<TokenPtr>& tokens, bool& skipping, int& skip_level, int& if_level, bool is_not) const {
  if (i + 1 >= tokens.size()) {
    std::string directive = is_not ? "ifndef" : "ifdef";
    return std::unexpected(InvalidDirectiveError("Incomplete #" + directive + " at line " +
                                                 std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[i + 1];

  if (!(id_token->GetStringType() == "IDENT")) {
    std::string directive = is_not ? "ifndef" : "ifdef";
    return std::unexpected(InvalidDirectiveError("Expected identifier after #" + directive + " at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  bool cond = macros_.count(id) > 0;

  if (is_not) {
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
  return {};
}

std::expected<void, PreprocessorError> TokenDirectivesProcessor::HandleElse(
    size_t& i, const std::vector<TokenPtr>& tokens, bool& skipping, int& skip_level, int& if_level) const {
  if (if_level == 0) {
    return std::unexpected(
        UnmatchedDirectiveError("Mismatched #else at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  if (!skipping) {
    skipping = true;
  } else if (skip_level == 1) {
    skipping = false;
  }

  i += 1;
  return {};
}

std::expected<void, PreprocessorError> TokenDirectivesProcessor::HandleEndif(
    size_t& i, const std::vector<TokenPtr>& tokens, bool& skipping, int& skip_level, int& if_level) const {
  if (if_level == 0) {
    return std::unexpected(
        UnmatchedDirectiveError("Mismatched #endif at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  if_level--;

  if (skip_level > 0) {
    skip_level--;
  }

  if (skip_level == 0) {
    skipping = false;
  }

  i += 1;
  return {};
}

std::vector<TokenPtr> TokenDirectivesProcessor::ExpandMacros(const std::vector<TokenPtr>& tokens) const {
  std::vector<TokenPtr> expanded;

  for (const TokenPtr& tok : tokens) {
    if (tok->GetStringType() == "IDENT" && macros_.count(tok->GetLexeme()) > 0) {
      const std::vector<TokenPtr>& exp_tokens = macros_.at(tok->GetLexeme());
      expanded.insert(expanded.end(), exp_tokens.begin(), exp_tokens.end());
    } else {
      expanded.push_back(tok);
    }
  }

  return expanded;
}
