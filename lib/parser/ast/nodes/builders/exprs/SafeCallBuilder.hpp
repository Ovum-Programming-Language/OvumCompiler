#ifndef PARSER_SAFECALLBUILDER_HPP_
#define PARSER_SAFECALLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/SafeCall.hpp"

namespace ovum::compiler::parser {

class SafeCallBuilder : public NodeBuilderBase<SafeCall> {
public:
  SafeCallBuilder& WithObject(std::unique_ptr<Expr> object);
  SafeCallBuilder& WithMethod(std::string method);
  SafeCallBuilder& AddArg(std::unique_ptr<Expr> argument);
  SafeCallBuilder& ClearArgs();
};

} // namespace ovum::compiler::parser

#endif // PARSER_SAFECALLBUILDER_HPP_
