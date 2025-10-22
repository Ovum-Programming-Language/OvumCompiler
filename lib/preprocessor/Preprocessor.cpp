#include "Preprocessor.hpp"

#include "token_processor_factory.hpp"

Preprocessor::Preprocessor(const PreprocessingParameters& parameters) :
    parameters_(parameters), token_processors_(TokenProcessorFactory::MakeTokenProcessors()) {
}

std::expected<std::vector<TokenPtr>, PreprocessorError> Preprocessor::Process(
    const std::vector<TokenPtr>& tokens) const {
  std::vector<TokenPtr> result = tokens;

  for (const auto& processor : token_processors_) {
    std::expected<std::vector<TokenPtr>, PreprocessorError> processor_result = processor->Process(result);

    if (!processor_result.has_value()) {
      return processor_result;
    }

    result = std::move(processor_result.value());
  }

  return {std::move(result)};
}
