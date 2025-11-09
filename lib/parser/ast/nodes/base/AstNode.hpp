#ifndef PARSER_ASTNODE_HPP_
#define PARSER_ASTNODE_HPP_

namespace ovum::compiler::parser {

class AstVisitor; // forward

class AstNode {
public:
  virtual ~AstNode() = default;

  // TODO: add positions......

  virtual void Accept(AstVisitor& visitor) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ASTNODE_HPP_
