#ifndef DIRECTIVE_HANDLER_HPP_
#define DIRECTIVE_HANDLER_HPP_

#include <expected>
#include <memory>
#include <unordered_set>
#include <vector>

#include "lib/lexer/tokens/Token.hpp"
#include "lib/preprocessor/PreprocessorError.hpp"

class DirectiveHandler {
public:
  virtual ~DirectiveHandler() = default;
  DirectiveHandler() = default;
  DirectiveHandler(const DirectiveHandler&) = delete;
  DirectiveHandler(DirectiveHandler&&) = default;
  DirectiveHandler& operator=(const DirectiveHandler&) = delete;
  DirectiveHandler& operator=(DirectiveHandler&&) = default;

  [[nodiscard]] virtual std::expected<void, PreprocessorError> Process(size_t& i,
                                                                       const std::vector<TokenPtr>& tokens,
                                                                       std::vector<TokenPtr>& result,
                                                                       std::unordered_set<std::string>& defined_symbols,
                                                                       bool& skipping,
                                                                       int& skip_level,
                                                                       int& if_level) = 0;

  void SetNext(std::unique_ptr<DirectiveHandler> next) {
    next_ = std::move(next);
  }

protected:
  std::unique_ptr<DirectiveHandler> next_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};

#endif // DIRECTIVE_HANDLER_HPP_
