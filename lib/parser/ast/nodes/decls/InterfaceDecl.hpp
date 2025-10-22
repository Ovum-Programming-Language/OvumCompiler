#ifndef INTERFACEDECL_HPP_
#define INTERFACEDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"

class InterfaceDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  std::string name_;
  std::vector<std::unique_ptr<Decl>> members_;
};

#endif // INTERFACEDECL_HPP_
