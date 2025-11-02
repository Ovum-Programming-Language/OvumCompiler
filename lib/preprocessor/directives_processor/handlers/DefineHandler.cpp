#include "DefineHandler.hpp"

namespace ovum::compiler::preprocessor {

void DefineHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> DefineHandler::Process(size_t& position,
                                                              const std::vector<TokenPtr>& tokens,
                                                              std::vector<TokenPtr>& processed_tokens,
                                                              std::unordered_set<std::string>& defined_symbols,
                                                              bool& skipping,
                                                              int& skip_level,
                                                              int& if_level,
                                                              std::vector<bool>& else_seen) {
  if (position >= tokens.size() || tokens[position]->GetLexeme() != "#define") {
    if (next_) {
      return next_->Process(position, tokens, processed_tokens, defined_symbols, skipping, skip_level, if_level, else_seen);
    }

    return {};
  }

  if (position + 1 >= tokens.size()) {
    return std::unexpected(InvalidDirectiveError("Incomplete #define at line " +
                                                 std::to_string(tokens[position]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[position + 1];

  if (id_token->GetStringType() != "IDENT") {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #define at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  if (position + 2 >= tokens.size()) {
    if (!skipping) {
      defined_symbols.insert(id);
    }

    position += 2;

    return {};
  } else if (tokens[position + 2]->GetStringType() == "NEWLINE" || tokens[position + 2]->GetStringType() == "EOF" ||
             tokens[position + 2]->GetLexeme() == ";") {
    if (!skipping) {
      defined_symbols.insert(id);
    }

    position += 3;

    return {};
  }

  return std::unexpected(InvalidDirectiveError("#define " + id + " has unexpected tokens after identifier at line " +
                                               std::to_string(tokens[position]->GetPosition().GetLine())));
}

} // namespace ovum::compiler::preprocessor
