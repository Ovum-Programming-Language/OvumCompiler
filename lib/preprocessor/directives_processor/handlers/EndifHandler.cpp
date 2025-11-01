#include "EndifHandler.hpp"

namespace ovum::compiler::preprocessor {

void EndifHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> EndifHandler::Process(size_t& i,
                                                             const std::vector<TokenPtr>& tokens,
                                                             std::vector<TokenPtr>& result,
                                                             std::unordered_set<std::string>& defined_symbols,
                                                             bool& skipping,
                                                             int& skip_level,
                                                             int& if_level) {
  if (i >= tokens.size() || tokens[i]->GetLexeme() != "#endif") {
    if (next_) {
      return next_->Process(i, tokens, result, defined_symbols, skipping, skip_level, if_level);
    }

    return {};
  }

  if (if_level == 0) {
    return std::unexpected(
        UnmatchedDirectiveError("Mismatched #endif at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  --if_level;

  if (skip_level > 0) {
    --skip_level;
  }

  if (skip_level == 0) {
    skipping = false;
  }

  ++i;

  return {};
}

} // namespace ovum::compiler::preprocessor
