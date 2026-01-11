#ifndef PARSER_BYTELITBUILDER_HPP_
#define PARSER_BYTELITBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/ByteLit.hpp"

namespace ovum::compiler::parser {

class ByteLitBuilder : public NodeBuilderBase<ByteLit> {
public:
  ByteLitBuilder& WithValue(uint8_t value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_BYTELITBUILDER_HPP_

