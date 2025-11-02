#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/tokens/SourceId.hpp"

class Module : public AstNode {
public:
  void Accept(AstVisitor& v) override;

  const std::string& Name() const noexcept;
  void SetName(std::string n);

  const SourceId& Source() const noexcept;
  void SetSource(SourceId id);

  const std::vector<std::unique_ptr<Decl>>& Decls() const noexcept;
  std::vector<std::unique_ptr<Decl>>& MutableDecls() noexcept;
  void AddDecl(std::unique_ptr<Decl> d);
  std::unique_ptr<Decl> ReleaseDecl(std::size_t index);

private:
  std::string name_;
  SourceId source_;
  std::vector<std::unique_ptr<Decl>> decls_;
};

#endif // MODULE_HPP_
