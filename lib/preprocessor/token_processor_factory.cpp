#include "token_processor_factory.hpp"

std::vector<std::unique_ptr<TokenProcessor>> TokenProcessorFactory::MakeTokenProcessors(
    PreprocessingParameters parameters) {
  std::vector<std::unique_ptr<TokenProcessor>> processors;

  auto directives_processor = std::make_unique<TokenDirectivesProcessor>(parameters.predefined_symbols);
  processors.push_back(std::move(directives_processor));

  auto import_processor = std::make_unique<TokenImportProcessor>(std::move(parameters));
  processors.push_back(std::move(import_processor));

  return processors;
}
