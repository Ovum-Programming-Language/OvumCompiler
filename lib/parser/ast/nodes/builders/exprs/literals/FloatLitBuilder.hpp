#ifndef PARSER_FLOATLITBUILDER_HPP_
#define PARSER_FLOATLITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/FloatLit.hpp"

namespace ovum::compiler::parser {

class FloatLitBuilder : public NodeBuilderBase<FloatLit> {
public:
  FloatLitBuilder& WithValue(double value);
  FloatLitBuilder& WithValue(long double value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_FLOATLITBUILDER_HPP_
