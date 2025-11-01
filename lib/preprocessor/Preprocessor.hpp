#ifndef PREPROCESSOR_PREPROCESSOR_HPP_
#define PREPROCESSOR_PREPROCESSOR_HPP_

#include <expected>
#include <memory>
#include <vector>

#include "PreprocessingParameters.hpp"
#include "TokenProcessor.hpp"

namespace ovum::compiler::preprocessor {

class Preprocessor {
public:
  explicit Preprocessor(const PreprocessingParameters& parameters);

  [[nodiscard]] std::expected<std::vector<TokenPtr>, PreprocessorError> Process();

private:
  PreprocessingParameters parameters_;
  std::vector<std::unique_ptr<TokenProcessor>> token_processors_;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSOR_PREPROCESSOR_HPP_
