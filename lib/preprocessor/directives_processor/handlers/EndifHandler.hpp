#ifndef PREPROCESSOR_ENDIF_HANDLER_HPP_
#define PREPROCESSOR_ENDIF_HANDLER_HPP_

#include "DirectiveHandler.hpp"
#include "lib/lexer/tokens/Token.hpp"

namespace ovum::compiler::preprocessor {

class EndifHandler : public DirectiveHandler {
public:
  [[nodiscard]] std::expected<void, PreprocessorError> Process(size_t& position,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& processed_tokens,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level,
                                                               std::vector<bool>& else_seen) override;

  void SetNext(std::unique_ptr<DirectiveHandler> next) override;

private:
  std::unique_ptr<DirectiveHandler> next_;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_ENDIF_HANDLER_HPP_
