#ifndef PARSER_GLOBALVARDECLBUILDER_HPP_
#define PARSER_GLOBALVARDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/GlobalVarDecl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class GlobalVarDeclBuilder : public NodeBuilderBase<GlobalVarDecl> {
public:
  GlobalVarDeclBuilder& WithVar(bool is_var);
  GlobalVarDeclBuilder& WithName(std::string name);
  GlobalVarDeclBuilder& WithType(TypeReference type);
  GlobalVarDeclBuilder& WithInit(std::unique_ptr<Expr> init);
};

} // namespace ovum::compiler::parser

#endif // PARSER_GLOBALVARDECLBUILDER_HPP_
