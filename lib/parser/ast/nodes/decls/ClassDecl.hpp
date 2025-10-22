#ifndef CLASSDECL_HPP_
#define CLASSDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

class ClassDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

private:
  std::string name_;
  std::vector<TypeReference> implements_;
  std::vector<std::unique_ptr<Decl>> members_; // Field/StaticField/Method/Call/Destructor
};

#endif // CLASSDECL_HPP_
