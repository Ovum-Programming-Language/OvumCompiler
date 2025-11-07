#include "Preprocessor.hpp"

#include <exception>
#include <fstream>
#include <utility>

#include "lib/lexer/Lexer.hpp"
#include "token_processor_factory.hpp"

namespace ovum::compiler::preprocessor {

Preprocessor::Preprocessor(const PreprocessingParameters& parameters) :
    parameters_(parameters), token_processors_(MakeTokenProcessors(parameters)) {
}

std::expected<std::vector<TokenPtr>, PreprocessorError> Preprocessor::Process() {
  std::filesystem::path file = parameters_.main_file;

  if (!std::filesystem::exists(file)) {
    return std::unexpected(FileNotFoundError(file.string()));
  }

  std::ifstream file_stream(file, std::ios::binary);

  if (!file_stream.is_open()) {
    return std::unexpected(FileReadError(file.string(), "Cannot open"));
  }

  std::string content((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());

  if (file_stream.bad() || file_stream.fail()) {
    return std::unexpected(FileReadError(file.string(), "Read error"));
  }

  lexer::Lexer lexer(content, false);
  std::vector<TokenPtr> tokens;

  try {
    tokens = std::move(lexer.Tokenize());
  } catch (const std::exception& e) {
    return std::unexpected(PreprocessorError(e.what()));
  }

  for (std::unique_ptr<TokenProcessor>& processor : token_processors_) {
    std::expected<std::vector<TokenPtr>, PreprocessorError> processor_result = std::move(processor->Process(tokens));

    if (!processor_result) {
      return processor_result;
    }

    tokens = std::move(processor_result.value());
  }

  return {std::move(tokens)};
}

} // namespace ovum::compiler::preprocessor
