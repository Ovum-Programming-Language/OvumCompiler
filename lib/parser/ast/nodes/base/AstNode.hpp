#ifndef ASTNODE_HPP_
#define ASTNODE_HPP_

class AstVisitor; // forward

class AstNode {
public:
  virtual ~AstNode() = default;

  virtual void Accept(AstVisitor& v) = 0;
};

#endif // ASTNODE_HPP_
