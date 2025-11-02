#ifndef INTERFACEDECL_HPP_
#define INTERFACEDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "InterfaceMethod.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"

class InterfaceDecl : public Decl {
public:
  void Accept(AstVisitor& v) override;

  const std::string& Name() const noexcept;
  void SetName(std::string n);

  const std::vector<std::unique_ptr<InterfaceMethod>>& Members() const noexcept;
  std::vector<std::unique_ptr<InterfaceMethod>>& MutableMembers() noexcept;

private:
  std::string name_;
  std::vector<std::unique_ptr<InterfaceMethod>> methods_;
};

#endif // INTERFACEDECL_HPP_
