#ifndef PARSER_WHILESTMTBUILDER_HPP_
#define PARSER_WHILESTMTBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/WhileStmt.hpp"

namespace ovum::compiler::parser {

class WhileStmtBuilder : public NodeBuilderBase<WhileStmt> {
public:
  WhileStmtBuilder& WithCondition(std::unique_ptr<Expr> cond);
  WhileStmtBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_WHILESTMTBUILDER_HPP_
