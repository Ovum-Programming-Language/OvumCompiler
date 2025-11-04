#include "token_processor_factory.hpp"

#include "directives_processor/TokenDirectivesProcessor.hpp"
#include "import_processor/TokenImportProcessor.hpp"

namespace ovum::compiler::preprocessor {

std::vector<std::unique_ptr<TokenProcessor>> MakeTokenProcessors(const PreprocessingParameters& parameters) {
  std::vector<std::unique_ptr<TokenProcessor>> processors;

  std::unique_ptr<TokenImportProcessor> import_processor =
      std::make_unique<TokenImportProcessor>(parameters.main_file, parameters.include_paths);
  processors.push_back(std::move(import_processor));

  std::unique_ptr<TokenDirectivesProcessor> directives_processor =
      std::make_unique<TokenDirectivesProcessor>(parameters.predefined_symbols);
  processors.push_back(std::move(directives_processor));

  return processors;
}

} // namespace ovum::compiler::preprocessor
