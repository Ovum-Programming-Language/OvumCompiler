#ifndef PARSER_EXPRSTMTBUILDER_HPP_
#define PARSER_EXPRSTMTBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/ExprStmt.hpp"

namespace ovum::compiler::parser {

class ExprStmtBuilder : public NodeBuilderBase<ExprStmt> {
public:
  ExprStmtBuilder& WithExpr(std::unique_ptr<Expr> expr);
};

} // namespace ovum::compiler::parser

#endif // PARSER_EXPRSTMTBUILDER_HPP_
