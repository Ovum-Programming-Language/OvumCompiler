#ifndef TOKENDIRECTIVESPROCESSOR_HPP_
#define TOKENDIRECTIVESPROCESSOR_HPP_

#include <expected>
#include <string>
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
  std::unordered_set<std::string> defined_symbols_;

  [[nodiscard]] std::expected<void, PreprocessorError> HandleDefine(size_t& i,
                                                                    const std::vector<TokenPtr>& tokens,
                                                                    bool skipping);

  [[nodiscard]] std::expected<void, PreprocessorError> HandleUndef(size_t& i,
                                                                   const std::vector<TokenPtr>& tokens,
                                                                   bool skipping);

  [[nodiscard]] std::expected<void, PreprocessorError> HandleIfdefOrIfndef(size_t& i,
                                                                           const std::vector<TokenPtr>& tokens,
                                                                           bool& skipping,
                                                                           int& skip_level,
                                                                           int& if_level,
                                                                           bool is_not) const;

  [[nodiscard]] std::expected<void, PreprocessorError> HandleElse(
      size_t& i, const std::vector<TokenPtr>& tokens, bool& skipping, int& skip_level, int& if_level) const;

  [[nodiscard]] std::expected<void, PreprocessorError> HandleEndif(
      size_t& i, const std::vector<TokenPtr>& tokens, bool& skipping, int& skip_level, int& if_level) const;
};

#endif // TOKENDIRECTIVESPROCESSOR_HPP_
