#ifndef PARSER_INTERFACEMETHODBUILDER_HPP_
#define PARSER_INTERFACEMETHODBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class InterfaceMethodBuilder : public NodeBuilderBase<InterfaceMethod> {
public:
  InterfaceMethodBuilder& WithName(std::string name);
  InterfaceMethodBuilder& AddParam(Param parameter);
  InterfaceMethodBuilder& WithParams(std::vector<Param> params);
  InterfaceMethodBuilder& WithReturnType(std::unique_ptr<TypeReference> type);
};

} // namespace ovum::compiler::parser

#endif // PARSER_INTERFACEMETHODBUILDER_HPP_
