#include "lib/parser/ast/nodes/class_members/DestructorDecl.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

#include <utility>

namespace ovum::compiler::parser {

void DestructorDecl::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

bool DestructorDecl::IsPublic() const noexcept {
  return is_public_;
}

void DestructorDecl::SetPublic(bool value) noexcept {
  is_public_ = value;
}

const Block* DestructorDecl::Body() const noexcept {
  return body_.get();
}

Block* DestructorDecl::MutableBody() noexcept {
  return body_.get();
}

void DestructorDecl::SetBody(std::unique_ptr<Block> block) {
  body_ = std::move(block);
}

std::unique_ptr<Block> DestructorDecl::ReleaseBody() {
  return std::move(body_);
}

} // namespace ovum::compiler::parser
