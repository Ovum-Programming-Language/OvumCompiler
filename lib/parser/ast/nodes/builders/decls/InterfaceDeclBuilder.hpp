#ifndef PARSER_INTERFACEDECLBUILDER_HPP_
#define PARSER_INTERFACEDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceDecl.hpp"
#include "lib/parser/ast/nodes/decls/InterfaceMethod.hpp"

namespace ovum::compiler::parser {

class InterfaceDeclBuilder : public NodeBuilderBase<InterfaceDecl> {
public:
  InterfaceDeclBuilder& WithName(std::string name);
  InterfaceDeclBuilder& AddMethod(std::unique_ptr<InterfaceMethod> method);
};

} // namespace ovum::compiler::parser

#endif // PARSER_INTERFACEDECLBUILDER_HPP_
