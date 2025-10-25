#ifndef TOKENDIRECTIVESPROCESSOR_HPP_
#define TOKENDIRECTIVESPROCESSOR_HPP_

#include <expected>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PreprocessorError.hpp"
#include "lib/lexer/tokens/Token.hpp"

class TokenDirectivesProcessor {
public:
  explicit TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(std::vector<TokenPtr> tokens);

private:
  std::unordered_map<std::string, std::string> macros_;

  [[nodiscard]] bool is_identifier(const TokenPtr& token) const;
};

#endif // TOKENDIRECTIVESPROCESSOR_HPP_
