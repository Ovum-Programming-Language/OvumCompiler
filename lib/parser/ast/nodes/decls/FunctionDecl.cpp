#include "FunctionDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

namespace ovum::compiler::parser {

void FunctionDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool FunctionDecl::IsPure() const noexcept {
  return is_pure_;
}

void FunctionDecl::SetPure(bool is_pure) noexcept {
  is_pure_ = is_pure;
}

const std::string& FunctionDecl::Name() const noexcept {
  return name_;
}

void FunctionDecl::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const std::vector<Param>& FunctionDecl::Params() const noexcept {
  return params_;
}

std::vector<Param>& FunctionDecl::MutableParams() noexcept {
  return params_;
}

void FunctionDecl::AddParam(Param param) {
  params_.emplace_back(std::move(param));
}

const TypeReference* FunctionDecl::ReturnType() const noexcept {
  return return_type_.get();
}
TypeReference* FunctionDecl::MutableReturnType() noexcept {
  return return_type_.get();
}

void FunctionDecl::SetReturnType(std::unique_ptr<TypeReference> type) {
  return_type_ = std::move(type);
}

std::unique_ptr<TypeReference> FunctionDecl::ReleaseReturnType() {
  return std::move(return_type_);
}

const Block* FunctionDecl::Body() const noexcept {
  return body_.get();
}

Block* FunctionDecl::MutableBody() noexcept {
  return body_.get();
}

void FunctionDecl::SetBody(std::unique_ptr<Block> block) {
  body_ = std::move(block);
}

std::unique_ptr<Block> FunctionDecl::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
