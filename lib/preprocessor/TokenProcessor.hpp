#ifndef PREPROCESSOR_TOKENPROCESSOR_HPP_
#define PREPROCESSOR_TOKENPROCESSOR_HPP_

#include <expected>
#include <vector>

#include <tokens/Token.hpp>

#include "PreprocessorError.hpp"

namespace ovum::compiler::preprocessor {

using TokenPtr = ovum::TokenPtr;

class TokenProcessor { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~TokenProcessor() = default;

  [[nodiscard]] virtual std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) = 0;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_PREPROCESSOR_TOKENPROCESSOR_HPP_
