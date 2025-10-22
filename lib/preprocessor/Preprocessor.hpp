#ifndef PREPROCESSOR_HPP_
#define PREPROCESSOR_HPP_

#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenProcessor.hpp"

class Preprocessor : public TokenProcessor {
public:
  Preprocessor(const PreprocessingParameters& parameters);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process(
      const std::vector<TokenPtr>& tokens) const override;

private:
  PreprocessingParameters parameters_;
  std::vector<std::unique_ptr<TokenProcessor>> token_processors_;
};

#endif // PREPROCESSOR_HPP_
