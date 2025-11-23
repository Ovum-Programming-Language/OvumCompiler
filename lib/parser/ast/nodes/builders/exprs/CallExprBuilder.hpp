#ifndef PARSER_CALLEXPRBUILDER_HPP_
#define PARSER_CALLEXPRBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/Call.hpp"

namespace ovum::compiler::parser {

class CallExprBuilder : public NodeBuilderBase<Call> {
public:
  CallExprBuilder& WithCallee(std::unique_ptr<Expr> callee);
  CallExprBuilder& AddArg(std::unique_ptr<Expr> argument);
};

} // namespace ovum::compiler::parser

#endif // PARSER_CALLEXPRBUILDER_HPP_
