#ifndef PARSER_STATICFIELDDECLBUILDER_HPP_
#define PARSER_STATICFIELDDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/class_members/StaticFieldDecl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class StaticFieldDeclBuilder : public NodeBuilderBase<StaticFieldDecl> {
public:
  StaticFieldDeclBuilder& WithPublic(bool is_public);
  StaticFieldDeclBuilder& WithIsPublic(bool is_public);
  StaticFieldDeclBuilder& WithVar(bool is_var);
  StaticFieldDeclBuilder& WithIsVar(bool is_var);
  StaticFieldDeclBuilder& WithName(std::string name);
  StaticFieldDeclBuilder& WithType(TypeReference type);
  StaticFieldDeclBuilder& WithInit(std::unique_ptr<Expr> init);
};

} // namespace ovum::compiler::parser

#endif // PARSER_STATICFIELDDECLBUILDER_HPP_
