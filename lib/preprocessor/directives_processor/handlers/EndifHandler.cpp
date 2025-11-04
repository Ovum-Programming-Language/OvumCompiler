#include "EndifHandler.hpp"

namespace ovum::compiler::preprocessor {

void EndifHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> EndifHandler::Process(size_t& position,
                                                             const std::vector<TokenPtr>& tokens,
                                                             std::vector<TokenPtr>& processed_tokens,
                                                             std::unordered_set<std::string>& defined_symbols,
                                                             bool& skipping,
                                                             int& skip_level,
                                                             int& if_level,
                                                             std::vector<bool>& else_seen) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#endif") {
    if (next_) {
      return next_->Process(
          position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level, else_seen);
    }

    return {};
  }

  if (if_level == 0) {
    return std::unexpected(UnmatchedDirectiveError("Mismatched #endif at line " +
                                                   std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  --if_level;
  else_seen.pop_back();

  if (skip_level > 0) {
    --skip_level;
  }

  if (skip_level == 0) {
    skipping = false;
  }

  if (tokens[position + 1]->GetStringType() == "NEWLINE" || tokens[position + 1]->GetLexeme() == ";") {
    position += 2;

    return {};
  } else if (tokens[position + 1]->GetStringType() == "EOF") {
    ++position;

    return {};
  }

  return std::unexpected(InvalidDirectiveError("#endif has unexpected tokens after identifier at line " +
                                               std::to_string(tokens[position]->GetPosition().GetLine())));
}

} // namespace ovum::compiler::preprocessor
