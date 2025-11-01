#include "ElseHandler.hpp"

std::expected<void, PreprocessorError> ElseHandler::Process(
    size_t& i, const std::vector<TokenPtr>& tokens, std::vector<TokenPtr>& result,
    std::unordered_set<std::string>& defined_symbols, bool& skipping, int& skip_level,
    int& if_level) {
  if (i >= tokens.size() || tokens[i]->GetLexeme() != "#else") {
    if (next_) {
      return next_->Process(i, tokens, result, defined_symbols, skipping, skip_level, if_level);
    }
    return {};
  }

  if (if_level == 0) {
    return std::unexpected(UnmatchedDirectiveError("Mismatched #else at line " +
                                                   std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  if (!skipping) {
    skipping = true;
  } else if (skip_level == 1) {
    skipping = false;
  }

  ++i;
  return {};
}
