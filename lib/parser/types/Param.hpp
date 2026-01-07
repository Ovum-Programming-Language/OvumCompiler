#ifndef PARSER_PARAM_HPP_
#define PARSER_PARAM_HPP_

#include <string>

#include "TypeReference.hpp"

namespace ovum::compiler::parser {

class Param {
public:
  Param(std::string name, TypeReference typeReference);

  [[nodiscard]] const std::string& GetName() const;

  [[nodiscard]] const TypeReference& GetType() const;

private:
  std::string name_;
  TypeReference reference_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_PARAM_HPP_
