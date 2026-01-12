#ifndef PARSER_UNARYBUILDER_HPP_
#define PARSER_UNARYBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/Unary.hpp"

namespace ovum::compiler::parser {

class UnaryBuilder : public NodeBuilderBase<Unary> {
public:
  UnaryBuilder& WithOp(const IUnaryOpTag& op);
  UnaryBuilder& WithOperand(std::unique_ptr<Expr> operand);
};

} // namespace ovum::compiler::parser

#endif // PARSER_UNARYBUILDER_HPP_
