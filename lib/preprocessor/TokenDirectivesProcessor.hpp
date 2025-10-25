#ifndef TOKENDIRECTIVESPROCESSOR_HPP_
#define TOKENDIRECTIVESPROCESSOR_HPP_

#include <expected>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PreprocessorError.hpp"
#include "TokenProcessor.hpp"
#include "lib/lexer/tokens/Token.hpp"

class TokenDirectivesProcessor : public TokenProcessor {
public:
  explicit TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) override;

private:
  std::unordered_map<std::string, std::vector<TokenPtr>> macros_;
};

#endif // TOKENDIRECTIVESPROCESSOR_HPP_
