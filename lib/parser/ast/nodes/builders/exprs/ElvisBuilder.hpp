#ifndef PARSER_ELVISBUILDER_HPP_
#define PARSER_ELVISBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/Elvis.hpp"

namespace ovum::compiler::parser {

class ElvisBuilder : public NodeBuilderBase<Elvis> {
public:
  ElvisBuilder& WithLhs(std::unique_ptr<Expr> lhs);
  ElvisBuilder& WithRhs(std::unique_ptr<Expr> rhs);
};

} // namespace ovum::compiler::parser

#endif // PARSER_ELVISBUILDER_HPP_
