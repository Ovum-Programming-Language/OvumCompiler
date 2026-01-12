#ifndef PARSER_INDEXACCESSBUILDER_HPP_
#define PARSER_INDEXACCESSBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/exprs/IndexAccess.hpp"

namespace ovum::compiler::parser {

class IndexAccessBuilder : public NodeBuilderBase<IndexAccess> {
public:
  IndexAccessBuilder& WithObject(std::unique_ptr<Expr> object);
  IndexAccessBuilder& WithIndex(std::unique_ptr<Expr> index_expr);
};

} // namespace ovum::compiler::parser

#endif // PARSER_INDEXACCESSBUILDER_HPP_
