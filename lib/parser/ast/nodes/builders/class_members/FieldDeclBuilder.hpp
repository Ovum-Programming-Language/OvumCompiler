#ifndef PARSER_FIELDDECLBUILDER_HPP_
#define PARSER_FIELDDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/class_members/FieldDecl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class FieldDeclBuilder : public NodeBuilderBase<FieldDecl> {
public:
  FieldDeclBuilder& WithPublic(bool is_public);
  FieldDeclBuilder& WithVar(bool is_var);
  FieldDeclBuilder& WithName(std::string name);
  FieldDeclBuilder& WithType(TypeReference type);
  FieldDeclBuilder& WithInit(std::unique_ptr<Expr> init);
};

} // namespace ovum::compiler::parser

#endif // PARSER_FIELDDECLBUILDER_HPP_
