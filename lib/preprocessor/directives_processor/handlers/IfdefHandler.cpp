#include "IfdefHandler.hpp"

namespace ovum::compiler::preprocessor {

void IfdefHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> IfdefHandler::Process(size_t& position,
                                                             const std::vector<TokenPtr>& tokens,
                                                             std::vector<TokenPtr>& processed_tokens,
                                                             std::unordered_set<std::string>& defined_symbols,
                                                             bool& skipping,
                                                             int& skip_level,
                                                             int& if_level,
                                                             std::vector<bool>& else_seen) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#ifdef") {
    if (next_) {
      return next_->Process(
          position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level, else_seen);
    }

    return {};
  }

  if (position + 1 >= tokens.size()) {
    return std::unexpected(InvalidDirectiveError("Incomplete #ifdef at line " +
                                                 std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[position + 1];

  if (id_token->GetStringType() != "IDENT") {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #ifdef at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();
  bool cond = defined_symbols.count(id) > 0;

  else_seen.push_back(false);
  ++if_level;

  if (skipping) {
    ++skip_level;
  } else {
    if (!cond) {
      skipping = true;
      skip_level = 1;
    }
  }

  position += 3;

  return {};
}

} // namespace ovum::compiler::preprocessor
