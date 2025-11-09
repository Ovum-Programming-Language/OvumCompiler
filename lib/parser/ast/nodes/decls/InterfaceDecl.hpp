#ifndef INTERFACEDECL_HPP_
#define INTERFACEDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "InterfaceMethod.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"

class InterfaceDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const std::vector<std::unique_ptr<InterfaceMethod>>& Members() const noexcept;
  std::vector<std::unique_ptr<InterfaceMethod>>& MutableMembers() noexcept;

  void AddMember(std::unique_ptr<InterfaceMethod> method);
  std::unique_ptr<InterfaceMethod> ReleaseMember(std::size_t index);

private:
  std::string name_;
  std::vector<std::unique_ptr<InterfaceMethod>> methods_;
};

#endif // INTERFACEDECL_HPP_
