#include "Preprocessor.hpp"

#include "token_processor_factory.hpp"

Preprocessor::Preprocessor(const PreprocessingParameters& parameters) :
    parameters_(parameters), token_processors_(TokenProcessorFactory::MakeTokenProcessors()) {
}

std::vector<TokenPtr> Preprocessor::Process(const std::vector<TokenPtr>& tokens) const {
  return {}; // TODO: Implement
}
