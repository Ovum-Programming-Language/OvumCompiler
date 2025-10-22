#ifndef PREPROCESSINGPARAMETERS_HPP_
#define PREPROCESSINGPARAMETERS_HPP_

#include <set>
#include <string>
#include <unordered_set>

struct PreprocessingParameters {
  std::set<std::string> include_paths;
  std::unordered_set<std::string> predefined_symbols;
};

#endif // PREPROCESSINGPARAMETERS_HPP_
