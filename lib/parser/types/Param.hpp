#ifndef PARAM_HPP_
#define PARAM_HPP_

#include <string>

#include "TypeReference.hpp"

class Param {
public:
  Param(std::string name, TypeReference typeReference);

  [[nodiscard]] std::string getName();

  [[nodiscard]] TypeReference getType();

private:
  std::string name;
  TypeReference reference;
};

#endif // PARAM_HPP_
