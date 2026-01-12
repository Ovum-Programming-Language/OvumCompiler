#ifndef PARSER_FUNCTIONDECLBUILDER_HPP_
#define PARSER_FUNCTIONDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/FunctionDecl.hpp"
#include "lib/parser/ast/nodes/stmts/Block.hpp"
#include "lib/parser/types/Param.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class FunctionDeclBuilder : public NodeBuilderBase<FunctionDecl> {
public:
  FunctionDeclBuilder& WithPure(bool is_pure);
  FunctionDeclBuilder& WithName(std::string name);
  FunctionDeclBuilder& AddParam(Param parameter);
  FunctionDeclBuilder& WithParams(std::vector<Param> params);
  FunctionDeclBuilder& WithReturnType(std::unique_ptr<TypeReference> type);
  FunctionDeclBuilder& WithBody(std::unique_ptr<Block> body);
};

} // namespace ovum::compiler::parser

#endif // PARSER_FUNCTIONDECLBUILDER_HPP_
