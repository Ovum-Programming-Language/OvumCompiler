#include "DefineHandler.hpp"

void DefineHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> DefineHandler::Process(size_t& i,
                                                              const std::vector<TokenPtr>& tokens,
                                                              std::vector<TokenPtr>& result,
                                                              std::unordered_set<std::string>& defined_symbols,
                                                              bool& skipping,
                                                              int& skip_level,
                                                              int& if_level) {
  if (i >= tokens.size() || tokens[i]->GetLexeme() != "#define") {
    if (next_) {
      return next_->Process(i, tokens, result, defined_symbols, skipping, skip_level, if_level);
    }
    return {};
  }

  if (i + 1 >= tokens.size()) {
    return std::unexpected(
        InvalidDirectiveError("Incomplete #define at line " + std::to_string(tokens[i]->GetPosition().GetLine())));
  }

  const TokenPtr& id_token = tokens[i + 1];

  if (id_token->GetStringType() != "IDENT") {
    return std::unexpected(InvalidDirectiveError("Expected identifier after #define at line " +
                                                 std::to_string(id_token->GetPosition().GetLine())));
  }

  std::string id = id_token->GetLexeme();

  if (i + 2 >= tokens.size()) {
    if (!skipping) {
      defined_symbols.insert(id);
    }
    i += 2;
    return {};
  } else if (tokens[i + 2]->GetStringType() == "NEWLINE" || tokens[i + 2]->GetStringType() == "EOF" ||
             tokens[i + 2]->GetLexeme() == ";") {
    if (!skipping) {
      defined_symbols.insert(id);
    }
    i += 3;
    return {};
  } else {
    return std::unexpected(InvalidDirectiveError("#define " + id + " has unexpected tokens after identifier at line " +
                                                 std::to_string(tokens[i]->GetPosition().GetLine())));
  }
}
