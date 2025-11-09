#ifndef PARSER_BREAKSTMT_HPP_
#define PARSER_BREAKSTMT_HPP_

#include "lib/parser/ast/nodes/base/Stmt.hpp"

namespace ovum::compiler::parser {

class BreakStmt : public Stmt {
public:
  void Accept(AstVisitor& visitor) override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_BREAKSTMT_HPP_
