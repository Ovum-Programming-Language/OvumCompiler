#include "TokenDirectivesProcessor.hpp"

#include <sstream>

TokenDirectivesProcessor::TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols) {
  for (const std::string& sym : predefined_symbols) {
    defined_symbols_.insert(sym);
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

  return {std::move(result)};
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

  if (i + 2 >= tokens.size()) {
    if (!skipping) {
      defined_symbols_.insert(id);
    }
    i += 2;
    return {};
  } else if (tokens[i + 2]->GetStringType() == "NEWLINE" || tokens[i + 2]->GetStringType() == "EOF" ||
             tokens[i + 2]->GetLexeme() == ";") {
    if (!skipping) {
      defined_symbols_.insert(id);
    }
    i += 3;
    return {};
  } else {
    return std::unexpected(InvalidDirectiveError("#define " + id + " has unexpected tokens after identifier at line " +
                                                 std::to_string(tokens[i]->GetPosition().GetLine())));
  }
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

  if (i + 2 >= tokens.size()) {
    if (!skipping) {
      defined_symbols_.erase(id);
    }
    i += 2;
    return {};
  }

  if (tokens[i + 2]->GetStringType() != "NEWLINE" && tokens[i + 2]->GetStringType() != "EOF" &&
      tokens[i + 2]->GetLexeme() != ";") {
    return std::unexpected(InvalidDirectiveError("#undef " + id + " has unexpected tokens after identifier at line " +
                                                 std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  if (!skipping) {
    defined_symbols_.erase(id);
  }

  i += 3;
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

  bool cond = defined_symbols_.count(id) > 0;

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
