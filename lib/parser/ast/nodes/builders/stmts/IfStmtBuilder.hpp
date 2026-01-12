#ifndef PARSER_IFSTMTBUILDER_HPP_
#define PARSER_IFSTMTBUILDER_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/Branch.hpp"
#include "lib/parser/ast/nodes/stmts/IfStmt.hpp"

namespace ovum::compiler::parser {

class IfStmtBuilder : public NodeBuilderBase<IfStmt> {
public:
  IfStmtBuilder& AddBranch(Branch branch);
  IfStmtBuilder& WithBranches(std::vector<Branch> branches);
  IfStmtBuilder& WithElse(std::unique_ptr<Block> else_block);
};

} // namespace ovum::compiler::parser

#endif // PARSER_IFSTMTBUILDER_HPP_
