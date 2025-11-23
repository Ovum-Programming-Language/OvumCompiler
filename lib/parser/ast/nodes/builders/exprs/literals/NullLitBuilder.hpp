#ifndef PARSER_NULLLITBUILDER_HPP_
#define PARSER_NULLLITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/NullLit.hpp"

namespace ovum::compiler::parser {

class NullLitBuilder : public NodeBuilderBase<NullLit> {
public:
  NullLitBuilder& Noop();
};

} // namespace ovum::compiler::parser

#endif // PARSER_NULLLITBUILDER_HPP_
