#include "MethodDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void MethodDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool MethodDecl::IsPublic() const noexcept {
  return is_public_;
}

void MethodDecl::SetPublic(bool is_public) noexcept {
  is_public_ = is_public;
}

bool MethodDecl::IsOverride() const noexcept {
  return is_override_;
}

void MethodDecl::SetOverride(bool is_override) noexcept {
  is_override_ = is_override;
}

bool MethodDecl::IsStatic() const noexcept {
  return is_static_;
}

void MethodDecl::SetStatic(bool is_static) noexcept {
  is_static_ = is_static;
}

bool MethodDecl::IsPure() const noexcept {
  return is_pure_;
}

void MethodDecl::SetPure(bool is_pure) noexcept {
  is_pure_ = is_pure;
}

const std::string& MethodDecl::Name() const noexcept {
  return name_;
}

void MethodDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const std::vector<Param>& MethodDecl::Params() const noexcept {
  return params_;
}

std::vector<Param>& MethodDecl::MutableParams() noexcept {
  return params_;
}

void MethodDecl::AddParam(Param param) {
  params_.emplace_back(std::move(param));
}

const TypeReference* MethodDecl::ReturnType() const noexcept {
  return ret_type_.get();
}

TypeReference* MethodDecl::MutableReturnType() noexcept {
  return ret_type_.get();
}

void MethodDecl::SetReturnType(std::unique_ptr<TypeReference> type) {
  ret_type_ = std::move(type);
}

std::unique_ptr<TypeReference> MethodDecl::ReleaseReturnType() {
  return std::move(ret_type_);
}

const Block* MethodDecl::Body() const noexcept {
  return body_.get();
}

Block* MethodDecl::MutableBody() noexcept {
  return body_.get();
}

void MethodDecl::SetBody(std::unique_ptr<Block> block) {
  body_ = std::move(block);
}

std::unique_ptr<Block> MethodDecl::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
