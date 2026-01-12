#ifndef PARSER_TYPEALIASDECLBUILDER_HPP_
#define PARSER_TYPEALIASDECLBUILDER_HPP_

#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/TypeAliasDecl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class TypeAliasDeclBuilder : public NodeBuilderBase<TypeAliasDecl> {
public:
  TypeAliasDeclBuilder& WithName(std::string name);
  TypeAliasDeclBuilder& WithAliasedType(TypeReference type);
};

} // namespace ovum::compiler::parser

#endif // PARSER_TYPEALIASDECLBUILDER_HPP_
