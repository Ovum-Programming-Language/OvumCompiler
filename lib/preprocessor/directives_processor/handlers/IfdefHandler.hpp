#ifndef PREPROCESSOR_IFDEF_HANDLER_HPP_
#define PREPROCESSOR_IFDEF_HANDLER_HPP_

#include "DirectiveHandler.hpp"
#include "lib/lexer/tokens/Token.hpp"

namespace ovum::compiler::preprocessor {

class IfdefHandler : public DirectiveHandler {
public:
  [[nodiscard]] std::expected<void, PreprocessorError> Process(size_t& i,
                                                               const std::vector<TokenPtr>& tokens,
                                                               std::vector<TokenPtr>& result,
                                                               std::unordered_set<std::string>& defined_symbols,
                                                               bool& skipping,
                                                               int& skip_level,
                                                               int& if_level) override;

  void SetNext(std::unique_ptr<DirectiveHandler> next) override;

private:
  std::unique_ptr<DirectiveHandler> next_;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_IFDEF_HANDLER_HPP_
