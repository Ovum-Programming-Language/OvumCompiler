#include <string_view>
#include <utility>
#include "Preprocessor.hpp"
#include "TokenImportProcessor.hpp"
#include "token_processor_factory.hpp"

Preprocessor::Preprocessor(PreprocessingParameters parameters) :
    parameters_(parameters), token_processors_(TokenProcessorFactory::MakeTokenProcessors(parameters)) {
}

std::expected<std::vector<TokenPtr>, PreprocessorError> Preprocessor::Process() {
  auto file = parameters_.main_file;
  if (!std::filesystem::exists(file)) {
    return std::unexpected(FileNotFoundError(file.string()));
  }

  std::ifstream ifs(file, std::ios::binary);
  if (!ifs.is_open()) {
    return std::unexpected(FileReadError(file.string(), "Cannot open"));
  }

  std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  if (ifs.bad() || ifs.fail()) {
    return std::unexpected(FileReadError(file.string(), "Read error"));
  }

  Lexer lexer(content, false);
  std::vector<TokenPtr> tokens = lexer.Tokenize();
  for (auto& processor : token_processors_) {
    auto processor_result = processor->Process(tokens);
    if (!processor_result) {
      return processor_result;
    }
    tokens = processor_result.value();
  }

  return {std::move(tokens)};
}
