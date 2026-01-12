#include "ModuleBuilder.hpp"

namespace ovum::compiler::parser {

ModuleBuilder& ModuleBuilder::WithName(std::string name) {
  node_->SetName(std::move(name));
  return *this;
}

ModuleBuilder& ModuleBuilder::WithSource(SourceId id) {
  node_->SetSource(std::move(id));
  return *this;
}

ModuleBuilder& ModuleBuilder::AddDecl(std::unique_ptr<Decl> declaration) {
  node_->AddDecl(std::move(declaration));
  return *this;
}

ModuleBuilder& ModuleBuilder::WithDecls(std::vector<std::unique_ptr<Decl>> decls) {
  for (auto& decl : decls) {
    node_->AddDecl(std::move(decl));
  }
  return *this;
}

} // namespace ovum::compiler::parser
