#ifndef PREPROCESSOR_PREPROCESSINGPARAMETERS_HPP_
#define PREPROCESSOR_PREPROCESSINGPARAMETERS_HPP_

#include <filesystem>
#include <set>
#include <string>
#include <unordered_set>

namespace ovum::compiler::preprocessor {

struct PreprocessingParameters {
  std::set<std::filesystem::path> include_paths;
  std::unordered_set<std::string> predefined_symbols;
  std::filesystem::path main_file;
};

} // namespace ovum::compiler::preprocessor

#endif // PREPROCESSINGPARAMETERS_HPP_
