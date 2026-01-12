#ifndef PARSER_INTLITBUILDER_HPP_
#define PARSER_INTLITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/IntLit.hpp"

namespace ovum::compiler::parser {

class IntLitBuilder : public NodeBuilderBase<IntLit> {
public:
  IntLitBuilder& WithValue(long long value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_INTLITBUILDER_HPP_
