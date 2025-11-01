#include "IfdefHandler.hpp"

void IfdefHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> IfdefHandler::Process(size_t& i,
                                                             const std::vector<TokenPtr>& tokens,
                                                             std::vector<TokenPtr>& result,
                                                             std::unordered_set<std::string>& defined_symbols,
                                                             bool& skipping,
                                                             int& skip_level,
                                                             int& if_level) {
  if (i >= tokens.size() || tokens[i]->GetLexeme() != "#ifdef") {
    if (next_) {
      return next_->Process(i, tokens, result, defined_symbols, skipping, skip_level, if_level);
    }

    return {};
  }

  if (i + 1 >= tokens.size()) {
    return std::unexpected(
        InvalidDirectiveError("Incomplete #ifdef at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[i + 1];

  if (id_token->GetStringType() != "IDENT") {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #ifdef at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();
  bool cond = defined_symbols.count(id) > 0;

  ++if_level;

  if (skipping) {
    ++skip_level;
  } else {
    if (!cond) {
      skipping = true;
      skip_level = 1;
    }
  }

  i += 2;

  return {};
}
