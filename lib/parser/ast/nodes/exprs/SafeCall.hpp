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

  [[nodiscard]] const Expr& Object() const noexcept;
  [[nodiscard]] Expr& MutableObject() noexcept;
  void SetObject(std::unique_ptr<Expr> obj);
  std::unique_ptr<Expr> ReplaceObject(std::unique_ptr<Expr> obj);

  [[nodiscard]] const std::string& Method() const noexcept;
  void SetMethod(std::string m);

  [[nodiscard]] const std::vector<std::unique_ptr<Expr>>& Args() const noexcept;
  [[nodiscard]] std::vector<std::unique_ptr<Expr>>& MutableArgs() noexcept;
  void AddArg(std::unique_ptr<Expr> e);
  void ClearArgs();

  [[nodiscard]] bool IsNullPropagating() const noexcept;

  void SetInferredType(TypeReference t);
  [[nodiscard]] const std::optional<TypeReference>& InferredType() const noexcept;

private:
  std::unique_ptr<Expr> object_;
  std::string method_;
  std::vector<std::unique_ptr<Expr>> args_;

  std::optional<TypeReference> inferred_type_;
};

#endif // SAFECALL_HPP_
