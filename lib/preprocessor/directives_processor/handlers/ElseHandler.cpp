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
                                                            int& if_level,
                                                            std::vector<bool>& else_seen) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#else") {
    if (next_) {
      return next_->Process(
          position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level, else_seen);
    }

    return {};
  }

  if (if_level == 0) {
    return std::unexpected(UnmatchedDirectiveError("Mismatched #else at line " +
                                                   std::to_string(tokens[position]->GetPosition().GetLine())));
  }
  if (else_seen[if_level]) {
    return std::unexpected(PreprocessorError("Duplicate #else directive in the same #if block"));
  }
  else_seen[if_level] = true;

  if (!skipping) {
    skipping = true;
  } else if (skip_level == 1) {
    skipping = false;
  }

  if (tokens[position + 1]->GetStringType() == "NEWLINE" || tokens[position + 1]->GetStringType() == "EOF" ||
      tokens[position + 1]->GetLexeme() == ";") {
    position += 3;
    return {};
  }

  return std::unexpected(InvalidDirectiveError("#else has unexpected tokens after identifier at line " +
                                               std::to_string(tokens[position]->GetPosition().GetLine())));
}

} // namespace ovum::compiler::preprocessor
