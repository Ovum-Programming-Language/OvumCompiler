#include "CallDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void CallDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool CallDecl::IsPublic() const noexcept {
  return is_public_;
}

void CallDecl::SetPublic(bool v) noexcept {
  is_public_ = v;
}

const std::vector<Param>& CallDecl::Params() const noexcept {
  return params_;
}

std::vector<Param>& CallDecl::MutableParams() noexcept {
  return params_;
}

void CallDecl::AddParam(Param param) {
  params_.emplace_back(std::move(param));
}

const TypeReference* CallDecl::ReturnType() const noexcept {
  return ret_type_.get();
}

TypeReference* CallDecl::MutableReturnType() noexcept {
  return ret_type_.get();
}

void CallDecl::SetReturnType(std::unique_ptr<TypeReference> type) {
  ret_type_ = std::move(type);
}

std::unique_ptr<TypeReference> CallDecl::ReleaseReturnType() {
  return std::move(ret_type_);
}

const Block* CallDecl::Body() const noexcept {
  return body_.get();
}

Block* CallDecl::MutableBody() noexcept {
  return body_.get();
}

void CallDecl::SetBody(std::unique_ptr<Block> block) {
  body_ = std::move(block);
}

std::unique_ptr<Block> CallDecl::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
