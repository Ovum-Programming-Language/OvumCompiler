#ifndef PARAM_HPP_
#define PARAM_HPP_

#include <string>

#include "TypeReference.hpp"

class Param {
public:
  Param(std::string name, TypeReference typeReference);

  [[nodiscard]] std::string GetName();

  [[nodiscard]] TypeReference GetType();

private:
  std::string name_;
  TypeReference reference_;
};

#endif // PARAM_HPP_
