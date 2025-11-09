#include "Module.hpp"

#include "lib/parser/ast/AstVisitor.hpp"

void Module::Accept(AstVisitor& visitor) {
  visitor.Visit(*this);
}

const std::string& Module::Name() const noexcept {
  return name_;
}

void Module::SetName(std::string new_name) {
  name_ = std::move(new_name);
}

const SourceId& Module::Source() const noexcept {
  return source_;
}

void Module::SetSource(SourceId id) {
  source_ = id;
}

const std::vector<std::unique_ptr<Decl>>& Module::Decls() const noexcept {
  return decls_;
}

std::vector<std::unique_ptr<Decl>>& Module::MutableDecls() noexcept {
  return decls_;
}

void Module::AddDecl(std::unique_ptr<Decl> decl) {
  decls_.emplace_back(std::move(decl));
}

std::unique_ptr<Decl> Module::ReleaseDecl(std::size_t index) {
  if (index >= decls_.size()) {
    return nullptr;
  }

  auto old = std::move(decls_[index]);
  decls_.erase(decls_.begin() + index);
  return std::move(old);
}
