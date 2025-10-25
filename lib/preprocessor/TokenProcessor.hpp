#ifndef TOKENPROCESSOR_HPP_
#define TOKENPROCESSOR_HPP_

#include <expected>
#include <vector>

#include "PreprocessorError.hpp"
#include "lib/lexer/tokens/Token.hpp"

class TokenProcessor { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~TokenProcessor() = default;

  [[nodiscard]] virtual std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) = 0;
};

#endif // TOKENPROCESSOR_HPP_
