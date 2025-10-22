#ifndef PREPROCESSINGPARAMETERS_HPP_
#define PREPROCESSINGPARAMETERS_HPP_

#include <filesystem>
#include <set>
#include <string>
#include <unordered_set>

struct PreprocessingParameters {
  std::set<std::filesystem::path> include_paths;
  std::unordered_set<std::string> predefined_symbols;
};

#endif // PREPROCESSINGPARAMETERS_HPP_
