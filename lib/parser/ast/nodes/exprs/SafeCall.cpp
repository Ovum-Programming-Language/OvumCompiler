#include "lib/parser/ast/nodes/exprs/SafeCall.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void SafeCall::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const Expr& SafeCall::Object() const noexcept {
  return *object_;
}

Expr& SafeCall::MutableObject() noexcept {
  return *object_;
}

void SafeCall::SetObject(std::unique_ptr<Expr> object_expr) {
  object_ = std::move(object_expr);
}

std::unique_ptr<Expr> SafeCall::ReplaceObject(std::unique_ptr<Expr> new_object) {
  auto old_object = std::move(object_);
  object_ = std::move(new_object);
  return old_object;
}

const std::string& SafeCall::Method() const noexcept {
  return method_;
}

void SafeCall::SetMethod(std::string method_name) {
  method_ = std::move(method_name);
}

const std::vector<std::unique_ptr<Expr>>& SafeCall::Args() const noexcept {
  return args_;
}

std::vector<std::unique_ptr<Expr>>& SafeCall::MutableArgs() noexcept {
  return args_;
}

void SafeCall::AddArg(std::unique_ptr<Expr> argument_expr) {
  args_.emplace_back(std::move(argument_expr));
}

void SafeCall::ClearArgs() {
  args_.clear();
}

bool SafeCall::IsNullPropagating() const noexcept {
  return true;
}

void SafeCall::SetInferredType(TypeReference inferred) {
  inferred_type_ = std::move(inferred);
}

const std::optional<TypeReference>& SafeCall::InferredType() const noexcept {
  return inferred_type_;
}

} // namespace ovum::compiler::parser
