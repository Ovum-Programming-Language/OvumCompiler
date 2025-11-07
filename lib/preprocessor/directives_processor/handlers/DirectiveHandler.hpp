#ifndef PREPROCESSOR_DIRECTIVE_HANDLER_HPP_
#define PREPROCESSOR_DIRECTIVE_HANDLER_HPP_

#include <expected>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "lib/preprocessor/PreprocessorError.hpp"
#include "lib/tokens/Token.hpp"

namespace ovum::compiler::preprocessor {

class DirectiveHandler { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~DirectiveHandler() = default;

  [[nodiscard]] virtual std::expected<void, PreprocessorError> Process(size_t& position,
                                                                       const std::vector<TokenPtr>& tokens,
                                                                       std::vector<TokenPtr>& processed_tokens,
                                                                       std::unordered_set<std::string>& defined_symbols,
                                                                       bool& skipping,
                                                                       int& skip_level,
                                                                       int& if_level,
                                                                       std::vector<bool>& else_seen) = 0;

  virtual void SetNext(std::unique_ptr<DirectiveHandler> next) = 0;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_DIRECTIVE_HANDLER_HPP_
