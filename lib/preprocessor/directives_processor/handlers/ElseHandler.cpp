#include "ElseHandler.hpp"

namespace ovum::compiler::preprocessor {

void ElseHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> ElseHandler::Process(size_t& position,
                                                            const std::vector<TokenPtr>& tokens,
                                                            std::vector<TokenPtr>& processed_tokens,
                                                            std::unordered_set<std::string>& defined_symbols,
                                                            bool& skipping,
                                                            int& skip_level,
                                                            int& if_level) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#else") {
    if (next_) {
      return next_->Process(position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level);
    }

    return {};
  }

  if (if_level == 0) {
    return std::unexpected(UnmatchedDirectiveError("Mismatched #else at line " +
                                                   std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  if (!skipping) {
    skipping = true;
  } else if (skip_level == 1) {
    skipping = false;
  }

  ++position;

  return {};
}

} // namespace ovum::compiler::preprocessor
