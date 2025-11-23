#ifndef PARSER_CALLDECLBUILDER_HPP_
#define PARSER_CALLDECLBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/class_members/CallDecl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class CallDeclBuilder : public NodeBuilderBase<CallDecl> {
public:
  CallDeclBuilder& WithPublic(bool is_public);
  CallDeclBuilder& AddParam(Param parameter);
  CallDeclBuilder& WithReturnType(std::unique_ptr<TypeReference> type);
  CallDeclBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_CALLDECLBUILDER_HPP_
