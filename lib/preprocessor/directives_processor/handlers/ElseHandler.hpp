#ifndef ELSE_HANDLER_HPP_
#define ELSE_HANDLER_HPP_

#include <expected>

#include "DirectiveHandler.hpp"
#include "lib/lexer/tokens/Token.hpp"

class ElseHandler : public DirectiveHandler {
public:
  [[nodiscard]] std::expected<void, PreprocessorError> Process(size_t& i,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& result,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) override;
};

#endif // ELSE_HANDLER_HPP_
