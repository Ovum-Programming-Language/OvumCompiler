#ifndef PARSER_METHODDECLBUILDER_HPP_
#define PARSER_METHODDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/class_members/MethodDecl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class MethodDeclBuilder : public NodeBuilderBase<MethodDecl> {
public:
  MethodDeclBuilder& WithPublic(bool is_public);
  MethodDeclBuilder& WithOverride(bool is_override);
  MethodDeclBuilder& WithStatic(bool is_static);
  MethodDeclBuilder& WithPure(bool is_pure);
  MethodDeclBuilder& WithName(std::string name);
  MethodDeclBuilder& AddParam(Param parameter);
  MethodDeclBuilder& WithReturnType(std::unique_ptr<TypeReference> type);
  MethodDeclBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_METHODDECLBUILDER_HPP_
