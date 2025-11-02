#include "UndefHandler.hpp"

namespace ovum::compiler::preprocessor {

void UndefHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> UndefHandler::Process(size_t& position,
                                                             const std::vector<TokenPtr>& tokens,
                                                             std::vector<TokenPtr>& processed_tokens,
                                                             std::unordered_set<std::string>& defined_symbols,
                                                             bool& skipping,
                                                             int& skip_level,
                                                             int& if_level,
                                                             std::vector<bool>& else_seen) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#undef") {
    if (next_) {
      return next_->Process(
          position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level, else_seen);
    }

    return {};
  }

  if (position + 1 >= tokens.size()) {
    return std::unexpected(InvalidDirectiveError("Incomplete #undef at line " +
                                                 std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[position + 1];

  if (id_token->GetStringType() != "IDENT") {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #undef at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  if (position + 2 >= tokens.size()) {
    if (!skipping) {
      defined_symbols.erase(id);
    }

    position += 2;
    return {};
  }

  if (tokens[position + 2]->GetStringType() != "NEWLINE" && tokens[position + 2]->GetStringType() != "EOF" &&
      tokens[position + 2]->GetLexeme() != ";") {
    return std::unexpected(InvalidDirectiveError("#undef " + id + " has unexpected tokens after identifier at line " +
                                                 std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  if (!skipping) {
    defined_symbols.erase(id);
  }

  position += 3;

  return {};
}

} // namespace ovum::compiler::preprocessor
