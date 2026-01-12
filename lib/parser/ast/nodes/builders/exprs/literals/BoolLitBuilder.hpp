#ifndef PARSER_BOOLLITBUILDER_HPP_
#define PARSER_BOOLLITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/BoolLit.hpp"

namespace ovum::compiler::parser {

class BoolLitBuilder : public NodeBuilderBase<BoolLit> {
public:
  BoolLitBuilder& WithValue(bool value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_BOOLLITBUILDER_HPP_
