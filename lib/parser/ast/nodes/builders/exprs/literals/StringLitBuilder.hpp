#ifndef PARSER_STRINGLITBUILDER_HPP_
#define PARSER_STRINGLITBUILDER_HPP_

#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/literals/StringLit.hpp"

namespace ovum::compiler::parser {

class StringLitBuilder : public NodeBuilderBase<StringLit> {
public:
  StringLitBuilder& WithValue(std::string value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_STRINGLITBUILDER_HPP_
