#include "TokenDirectivesProcessor.hpp"

#include <cstdint>

#include "TokenDirectivesProcessor.hpp"
#include "directives_chain_factory.hpp"

namespace ovum::compiler::preprocessor {

TokenDirectivesProcessor::TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols) {
  for (const std::string& symbol : predefined_symbols) {
    defined_symbols_.insert(symbol);
  }

  directives_chain_ = CreateDirectivesChain();
}

std::expected<std::vector<TokenPtr>, PreprocessorError> TokenDirectivesProcessor::Process(
    const std::vector<TokenPtr>& tokens) {
  std::vector<TokenPtr> processed_tokens;
  size_t token_index = 0;
  bool skipping = false;
  int32_t skip_level = 0;
  int32_t if_level = 0;

  while (token_index < tokens.size()) {
    std::expected<void, PreprocessorError> handler_result = directives_chain_->Process(
        token_index, tokens, processed_tokens, defined_symbols_, skipping, skip_level, if_level, else_seen_);

    if (!handler_result) {
      return std::unexpected(handler_result.error());
    }
  }

  if (skip_level > 0 || if_level > 0) {
    return std::unexpected(UnmatchedDirectiveError("Unmatched #if directive"));
  }

  if (!else_seen_.empty()) {
    return std::unexpected(UnmatchedDirectiveError("Unmatched #if directive (else_seen stack not empty)"));
  }

  return {std::move(processed_tokens)};
}

} // namespace ovum::compiler::preprocessor
