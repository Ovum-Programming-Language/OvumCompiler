#ifndef PARSER_BLOCKBUILDER_HPP_
#define PARSER_BLOCKBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"

namespace ovum::compiler::parser {

class BlockBuilder : public NodeBuilderBase<Block> {
public:
  BlockBuilder& Append(std::unique_ptr<Stmt> statement);
  BlockBuilder& WithStatements(std::vector<std::unique_ptr<Stmt>> statements);
};

} // namespace ovum::compiler::parser

#endif // PARSER_BLOCKBUILDER_HPP_
