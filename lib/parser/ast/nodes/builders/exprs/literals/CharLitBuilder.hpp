#ifndef PARSER_CHARLITBUILDER_HPP_
#define PARSER_CHARLITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/CharLit.hpp"

namespace ovum::compiler::parser {

class CharLitBuilder : public NodeBuilderBase<CharLit> {
public:
  CharLitBuilder& WithValue(char value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_CHARLITBUILDER_HPP_
