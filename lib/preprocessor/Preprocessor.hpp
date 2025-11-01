#ifndef PREPROCESSOR_HPP_
#define PREPROCESSOR_HPP_

#include <expected>
#include <fstream>
#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenProcessor.hpp"
#include "directives_processor/TokenDirectivesProcessor.hpp"
#include "import_processor/TokenImportProcessor.hpp"
#include "token_processor_factory.hpp"

class Preprocessor {
public:
  explicit Preprocessor(const PreprocessingParameters& parameters);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process();

private:
  PreprocessingParameters parameters_;
  std::vector<std::unique_ptr<TokenProcessor>> token_processors_;
};

#endif // PREPROCESSOR_HPP_
