#ifndef PARSER_CONTINUESTMT_HPP_
#define PARSER_CONTINUESTMT_HPP_

#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class ContinueStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_CONTINUESTMT_HPP_
