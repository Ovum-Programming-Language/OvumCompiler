#ifndef PARSER_FIELDACCESSBUILDER_HPP_
#define PARSER_FIELDACCESSBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/FieldAccess.hpp"

namespace ovum::compiler::parser {

class FieldAccessBuilder : public NodeBuilderBase<FieldAccess> {
public:
  FieldAccessBuilder& WithObject(std::unique_ptr<Expr> object);
  FieldAccessBuilder& WithName(std::string name);
};

} // namespace ovum::compiler::parser

#endif // PARSER_FIELDACCESSBUILDER_HPP_
