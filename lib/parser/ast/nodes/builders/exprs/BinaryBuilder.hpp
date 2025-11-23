#ifndef PARSER_BINARYBUILDER_HPP_
#define PARSER_BINARYBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/Binary.hpp"

namespace ovum::compiler::parser {

class BinaryBuilder : public NodeBuilderBase<Binary> {
public:
  BinaryBuilder& WithOp(const IBinaryOpTag& op);
  BinaryBuilder& WithLhs(std::unique_ptr<Expr> lhs);
  BinaryBuilder& WithRhs(std::unique_ptr<Expr> rhs);
};

} // namespace ovum::compiler::parser

#endif // PARSER_BINARYBUILDER_HPP_
