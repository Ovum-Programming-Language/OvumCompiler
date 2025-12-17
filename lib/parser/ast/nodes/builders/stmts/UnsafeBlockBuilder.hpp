#ifndef PARSER_UNSAFEBLOCKBUILDER_HPP_
#define PARSER_UNSAFEBLOCKBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/ast/nodes/stmts/UnsafeBlock.hpp"

namespace ovum::compiler::parser {

class UnsafeBlockBuilder : public NodeBuilderBase<UnsafeBlock> {
public:
  UnsafeBlockBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_UNSAFEBLOCKBUILDER_HPP_
