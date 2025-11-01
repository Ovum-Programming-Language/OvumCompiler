#ifndef DEFAULT_HANDLER_HPP_
#define DEFAULT_HANDLER_HPP_

#include <expected>
#include <memory>

#include "DirectiveHandler.hpp"
#include "lib/lexer/tokens/Token.hpp"

class DefaultHandler : public DirectiveHandler {
public:
  [[nodiscard]] std::expected<void, PreprocessorError> Process(size_t& i,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& result,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) override;
};

#endif // DEFAULT_HANDLER_HPP_
