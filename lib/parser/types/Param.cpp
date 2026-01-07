#include "Param.hpp"

namespace ovum::compiler::parser {

Param::Param(std::string name, TypeReference typeReference) :
    name_(std::move(name)), reference_(std::move(typeReference)) {
}

const std::string& Param::GetName() const {
  return name_;
}

const TypeReference& Param::GetType() const {
  return reference_;
}

} // namespace ovum::compiler::parser
