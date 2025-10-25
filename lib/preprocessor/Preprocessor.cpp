#include <string_view>
#include "Preprocessor.hpp"
#include "TokenImportProcessor.hpp"
#include "token_processor_factory.hpp"

Preprocessor::Preprocessor(PreprocessingParameters parameters) :
    parameters_(std::move(parameters)), import_processor_(std::make_unique<TokenImportProcessor>(parameters_)),
    directives_processor_(std::make_unique<TokenDirectivesProcessor>(parameters.predefined_symbols)) {
}

std::expected<std::vector<TokenPtr>, PreprocessorError> Preprocessor::Process() {
  auto import_result = import_processor_->Process();
  if (!import_result) {
    return import_result;
  }

  std::vector<TokenPtr> result = std::move(import_result.value());

  auto directives_result = directives_processor_->Process(result);
  if (!directives_result) {
    return directives_result;
  }

  result = std::move(directives_result.value());

  return {std::move(result)};
}
