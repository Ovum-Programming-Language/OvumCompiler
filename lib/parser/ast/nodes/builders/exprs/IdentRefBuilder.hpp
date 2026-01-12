#ifndef PARSER_IDENTREFBUILDER_HPP_
#define PARSER_IDENTREFBUILDER_HPP_

#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/IdentRef.hpp"

namespace ovum::compiler::parser {

class IdentRefBuilder : public NodeBuilderBase<IdentRef> {
public:
  IdentRefBuilder& WithName(std::string name);
};

} // namespace ovum::compiler::parser

#endif // PARSER_IDENTREFBUILDER_HPP_
