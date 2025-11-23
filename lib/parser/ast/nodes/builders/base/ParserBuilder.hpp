#ifndef PARSER_PARSERBUILDER_HPP_
#define PARSER_PARSERBUILDER_HPP_

#include "BuilderTraits.hpp"

namespace ovum::compiler::parser {

class ParserBuilder {
public:
  ParserBuilder() = delete;

  template<class NodeT>
  using BuilderType = typename BuilderFor<NodeT>::type;

  template<class NodeT>
  static BuilderType<NodeT> Make() {
    static_assert(kHasBuilderV<NodeT>, "No builder registered for this AST node type");
    return BuilderType<NodeT>{};
  }
};

} // namespace ovum::compiler::parser

#endif // PARSER_PARSERBUILDER_HPP_
