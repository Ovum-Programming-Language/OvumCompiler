#ifndef PARSER_MODULE_HPP_
#define PARSER_MODULE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/tokens/SourceId.hpp"

namespace ovum::compiler::parser {

class Module : public AstNode {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string new_name);

  [[nodiscard]] const SourceId& Source() const noexcept;
  void SetSource(SourceId id);

  [[nodiscard]] const std::vector<std::unique_ptr<Decl>>& Decls() const noexcept;
  std::vector<std::unique_ptr<Decl>>& MutableDecls() noexcept;
  void AddDecl(std::unique_ptr<Decl> decl);
  std::unique_ptr<Decl> ReleaseDecl(std::size_t index);

private:
  std::string name_;
  SourceId source_;
  std::vector<std::unique_ptr<Decl>> decls_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MODULE_HPP_
