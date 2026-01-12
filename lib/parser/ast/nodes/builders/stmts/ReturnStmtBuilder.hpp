#ifndef PARSER_RETURNSTMTBUILDER_HPP_
#define PARSER_RETURNSTMTBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/ReturnStmt.hpp"

namespace ovum::compiler::parser {

class ReturnStmtBuilder : public NodeBuilderBase<ReturnStmt> {
public:
  ReturnStmtBuilder& WithValue(std::unique_ptr<Expr> value);
};

} // namespace ovum::compiler::parser

#endif // PARSER_RETURNSTMTBUILDER_HPP_
