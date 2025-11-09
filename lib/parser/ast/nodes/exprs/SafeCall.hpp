#ifndef SAFECALL_HPP_
#define SAFECALL_HPP_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/types/TypeReference.hpp"

class AstVisitor;

class SafeCall : public Expr {
public:
  void Accept(AstVisitor& v) override;

  const Expr& Object() const noexcept;
  Expr& MutableObject() noexcept;
  void SetObject(std::unique_ptr<Expr> object_expr);
  std::unique_ptr<Expr> ReplaceObject(std::unique_ptr<Expr> new_object);

  const std::string& Method() const noexcept;
  void SetMethod(std::string method_name);

  const std::vector<std::unique_ptr<Expr>>& Args() const noexcept;
  std::vector<std::unique_ptr<Expr>>& MutableArgs() noexcept;
  void AddArg(std::unique_ptr<Expr> argument_expr);
  void ClearArgs();

  bool IsNullPropagating() const noexcept;

  void SetInferredType(TypeReference inferred);
  const std::optional<TypeReference>& InferredType() const noexcept;

private:
  std::unique_ptr<Expr> object_;
  std::string method_;
  std::vector<std::unique_ptr<Expr>> args_;
  std::optional<TypeReference> inferred_type_;
};

#endif // SAFECALL_HPP_
