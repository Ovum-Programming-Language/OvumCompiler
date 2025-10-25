#ifndef PREPROCESSOR_HPP_
#define PREPROCESSOR_HPP_

#include <expected>
#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenDirectivesProcessor.hpp"
#include "TokenImportProcessor.hpp"
#include "TokenProcessor.hpp"

class Preprocessor : public TokenProcessor {
public:
  explicit Preprocessor(PreprocessingParameters parameters);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process();

private:
  PreprocessingParameters parameters_;

  std::unique_ptr<TokenDirectivesProcessor> directives_processor_;

  std::unique_ptr<TokenImportProcessor> import_processor_;
};

#endif // PREPROCESSOR_HPP_
