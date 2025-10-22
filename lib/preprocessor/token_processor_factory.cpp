#include "token_processor_factory.hpp"

std::vector<std::unique_ptr<TokenProcessor>> TokenProcessorFactory::MakeTokenProcessors() {
  return {}; // TODO: Add creation of all low-level token processors (maybe with changing the signature of the function)
}
