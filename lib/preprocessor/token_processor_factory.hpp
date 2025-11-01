#ifndef TOKEN_PROCESSOR_FACTORY_HPP_
#define TOKEN_PROCESSOR_FACTORY_HPP_

#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenProcessor.hpp"
#include "directives_processor/TokenDirectivesProcessor.hpp"
#include "import_processor/TokenImportProcessor.hpp"

namespace TokenProcessorFactory {

std::vector<std::unique_ptr<TokenProcessor>> MakeTokenProcessors(const PreprocessingParameters& parameters);

}; // namespace TokenProcessorFactory

#endif // TOKEN_PROCESSOR_FACTORY_HPP_
