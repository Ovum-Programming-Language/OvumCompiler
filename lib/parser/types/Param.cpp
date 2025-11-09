#include "lib/parser/types/Param.hpp"

namespace ovum::compiler::parser {

Param::Param(std::string name, TypeReference typeReference) :
    name_(std::move(name)), reference_(std::move(typeReference)) {
}

std::string Param::GetName() {
  return name_;
}

TypeReference Param::GetType() {
  return reference_;
}

} // namespace ovum::compiler::parser
