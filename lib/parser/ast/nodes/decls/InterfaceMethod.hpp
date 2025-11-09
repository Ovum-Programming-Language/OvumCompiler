#ifndef INTERFACEMETHOD_HPP_
#define INTERFACEMETHOD_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

class InterfaceMethod : public Decl {
public:
  struct Param {
    std::string name;
    TypeReference type;
  };

  void Accept(AstVisitor& visitor) override;

  const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  const std::vector<Param>& Params() const noexcept;
  std::vector<Param>& MutableParams() noexcept;

  const TypeReference* ReturnType() const noexcept;
  void SetReturnType(std::unique_ptr<TypeReference> type);

private:
  std::string name_;
  std::vector<Param> params_;
  std::unique_ptr<TypeReference> ret_type_;
};

#endif // INTERFACEMETHOD_HPP_
