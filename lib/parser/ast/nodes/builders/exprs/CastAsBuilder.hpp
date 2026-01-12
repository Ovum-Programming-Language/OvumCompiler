#ifndef PARSER_CASTASBUILDER_HPP_
#define PARSER_CASTASBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/CastAs.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class CastAsBuilder : public NodeBuilderBase<CastAs> {
public:
  CastAsBuilder& WithExpr(std::unique_ptr<Expr> expression);
  CastAsBuilder& WithType(TypeReference type);
};

} // namespace ovum::compiler::parser

#endif // PARSER_CASTASBUILDER_HPP_
