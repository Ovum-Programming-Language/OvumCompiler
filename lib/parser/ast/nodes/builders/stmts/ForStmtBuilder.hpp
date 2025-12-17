#ifndef PARSER_FORSTMTBUILDER_HPP_
#define PARSER_FORSTMTBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/ForStmt.hpp"

namespace ovum::compiler::parser {

class ForStmtBuilder : public NodeBuilderBase<ForStmt> {
public:
  ForStmtBuilder& WithIteratorName(std::string name);
  ForStmtBuilder& WithIteratorExpr(std::unique_ptr<Expr> expr);
  ForStmtBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_FORSTMTBUILDER_HPP_
