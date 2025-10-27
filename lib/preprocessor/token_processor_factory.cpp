#include "token_processor_factory.hpp"

std::vector<std::unique_ptr<TokenProcessor>> TokenProcessorFactory::MakeTokenProcessors(
    const PreprocessingParameters& parameters) {
  std::vector<std::unique_ptr<TokenProcessor>> processors;

  std::unique_ptr<TokenImportProcessor> import_processor =
      std::make_unique<TokenImportProcessor>(parameters.main_file, parameters.include_paths);
  processors.push_back(std::move(import_processor));

  std::unique_ptr<TokenDirectivesProcessor> directives_processor =
      std::make_unique<TokenDirectivesProcessor>(parameters.predefined_symbols);
  processors.push_back(std::move(directives_processor));

  return processors;
}
