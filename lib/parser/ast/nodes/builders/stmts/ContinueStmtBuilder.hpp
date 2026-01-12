#ifndef PARSER_CONTINUESTMTBUILDER_HPP_
#define PARSER_CONTINUESTMTBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/ContinueStmt.hpp"

namespace ovum::compiler::parser {

class ContinueStmtBuilder : public NodeBuilderBase<ContinueStmt> {
public:
  ContinueStmtBuilder& Noop();
};

} // namespace ovum::compiler::parser

#endif // PARSER_CONTINUESTMTBUILDER_HPP_
