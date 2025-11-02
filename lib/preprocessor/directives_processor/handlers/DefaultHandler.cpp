#include "DefaultHandler.hpp"

namespace ovum::compiler::preprocessor {

void DefaultHandler::SetNext(std::unique_ptr<DirectiveHandler> next) {
  next_ = std::move(next);
}

std::expected<void, PreprocessorError> DefaultHandler::Process(size_t& position,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& processed_tokens,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) {
  if (position >= tokens.size()) {
    return {};
  }

  const TokenPtr& token = tokens[position];

  if (!skipping) {
    processed_tokens.push_back(token);
  }

  ++position;

  return {};
}

} // namespace ovum::compiler::preprocessor
