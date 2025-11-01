#ifndef TOKENDIRECTIVESPROCESSOR_HPP_
#define TOKENDIRECTIVESPROCESSOR_HPP_

#include <expected>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "directives_chain_factory.hpp"
#include "lib/lexer/tokens/Token.hpp"
#include "lib/preprocessor/PreprocessorError.hpp"
#include "lib/preprocessor/TokenProcessor.hpp"

class TokenDirectivesProcessor : public TokenProcessor {
public:
  explicit TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) override;

private:
  std::unordered_set<std::string> defined_symbols_;
  std::unique_ptr<DirectiveHandler> directives_chain_;
};

#endif // TOKENDIRECTIVESPROCESSOR_HPP_
