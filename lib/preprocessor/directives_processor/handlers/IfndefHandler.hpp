#ifndef IFNDEF_HANDLER_HPP_
#define IFNDEF_HANDLER_HPP_

#include <expected>

#include "DirectiveHandler.hpp"
#include "lib/lexer/tokens/Token.hpp"

class IfndefHandler : public DirectiveHandler {
public:
  [[nodiscard]] std::expected<void, PreprocessorError> Process(size_t& i,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& result,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) override;
};

#endif // IFNDEF_HANDLER_HPP_
