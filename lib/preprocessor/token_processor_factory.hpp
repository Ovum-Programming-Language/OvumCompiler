#ifndef TOKEN_PROCESSOR_FACTORY_HPP_
#define TOKEN_PROCESSOR_FACTORY_HPP_

#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenProcessor.hpp"

namespace ovum::compiler::preprocessor {

std::vector<std::unique_ptr<TokenProcessor>> MakeTokenProcessors(const PreprocessingParameters& parameters);

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_TOKEN_PROCESSOR_FACTORY_HPP_
