#ifndef PARSER_NAMESPACEREFBUILDER_HPP_
#define PARSER_NAMESPACEREFBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/NamespaceRef.hpp"

namespace ovum::compiler::parser {

class NamespaceRefBuilder : public NodeBuilderBase<NamespaceRef> {
public:
  NamespaceRefBuilder& WithNamespace(std::unique_ptr<Expr> ns);
  NamespaceRefBuilder& WithName(std::string name);
};

} // namespace ovum::compiler::parser

#endif // PARSER_NAMESPACEREFBUILDER_HPP_
