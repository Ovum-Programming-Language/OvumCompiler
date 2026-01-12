#ifndef PARSER_BREAKSTMTBUILDER_HPP_
#define PARSER_BREAKSTMTBUILDER_HPP_

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/BreakStmt.hpp"

namespace ovum::compiler::parser {

class BreakStmtBuilder : public NodeBuilderBase<BreakStmt> {
public:
  BreakStmtBuilder& Noop();
};

} // namespace ovum::compiler::parser

#endif // PARSER_BREAKSTMTBUILDER_HPP_
