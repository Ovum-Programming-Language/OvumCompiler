#ifndef PARSER_ASSIGNBUILDER_HPP_
#define PARSER_ASSIGNBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/Assign.hpp"
#include "lib/parser/ast/nodes/exprs/tags/IAssignOpTag.hpp"

namespace ovum::compiler::parser {

class AssignBuilder : public NodeBuilderBase<Assign> {
public:
  AssignBuilder& WithKind(const IAssignOpTag& kind);
  AssignBuilder& WithTarget(std::unique_ptr<Expr> target);
  AssignBuilder& WithValue(std::unique_ptr<Expr> value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_ASSIGNBUILDER_HPP_
