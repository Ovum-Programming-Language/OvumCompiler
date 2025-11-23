#ifndef PARSER_DESTRUCTORDECLBUILDER_HPP_
#define PARSER_DESTRUCTORDECLBUILDER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"

namespace ovum::compiler::parser {

class DestructorDeclBuilder : public NodeBuilderBase<DestructorDecl> {
public:
  DestructorDeclBuilder& WithPublic(bool is_public);
  DestructorDeclBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_DESTRUCTORDECLBUILDER_HPP_
