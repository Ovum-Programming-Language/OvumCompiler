#ifndef PARSER_THISEXPRBUILDER_HPP_
#define PARSER_THISEXPRBUILDER_HPP_

#include "ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "ast/nodes/exprs/ThisExpr.hpp"

namespace ovum::compiler::parser {

class ThisExprBuilder : public NodeBuilderBase<ThisExpr> {
public:
  ThisExprBuilder& Noop();
};

} // namespace ovum::compiler::parser

#endif // PARSER_THISEXPRBUILDER_HPP_
