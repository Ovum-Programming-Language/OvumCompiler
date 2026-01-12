#ifndef PARSER_VARDECLSTMTBUILDER_HPP_
#define PARSER_VARDECLSTMTBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/stmts/VarDeclStmt.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class VarDeclStmtBuilder : public NodeBuilderBase<VarDeclStmt> {
public:
  VarDeclStmtBuilder& WithVar(bool is_var);
  VarDeclStmtBuilder& WithIsVar(bool is_var);
  VarDeclStmtBuilder& WithName(std::string name);
  VarDeclStmtBuilder& WithType(TypeReference type);
  VarDeclStmtBuilder& WithInit(std::unique_ptr<Expr> init);
};

} // namespace ovum::compiler::parser

#endif // PARSER_VARDECLSTMTBUILDER_HPP_
