#include "DefaultHandler.hpp"

void DefaultHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> DefaultHandler::Process(size_t& i,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& result,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) {
  if (i >= tokens.size()) {
    return {};
  }

  const TokenPtr& token = tokens[i];

  if (!skipping) {
    result.push_back(token);
  }

  ++i;

  return {};
}
