#ifndef PREPROCESSOR_TOKENDIRECTIVESPROCESSOR_HPP_
#define PREPROCESSOR_TOKENDIRECTIVESPROCESSOR_HPP_

#include <expected>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <tokens/Token.hpp>

#include "lib/preprocessor/PreprocessorError.hpp"
#include "lib/preprocessor/TokenProcessor.hpp"
#include "lib/preprocessor/directives_processor/handlers/DirectiveHandler.hpp"

namespace ovum::compiler::preprocessor {

class TokenDirectivesProcessor : public TokenProcessor {
public:
  explicit TokenDirectivesProcessor(const std::unordered_set<std::string>& predefined_symbols);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) override;

private:
  std::unordered_set<std::string> defined_symbols_;
  std::unique_ptr<DirectiveHandler> directives_chain_;
  std::vector<bool> else_seen_;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_TOKENDIRECTIVESPROCESSOR_HPP_
