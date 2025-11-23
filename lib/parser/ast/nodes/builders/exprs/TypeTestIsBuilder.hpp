#ifndef PARSER_TYPETESTISBUILDER_HPP_
#define PARSER_TYPETESTISBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/TypeTestIs.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class TypeTestIsBuilder : public NodeBuilderBase<TypeTestIs> {
public:
  TypeTestIsBuilder& WithExpr(std::unique_ptr<Expr> expression);
  TypeTestIsBuilder& WithType(TypeReference type);
};

} // namespace ovum::compiler::parser

#endif // PARSER_TYPETESTISBUILDER_HPP_
