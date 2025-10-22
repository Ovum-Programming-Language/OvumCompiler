#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <memory>
#include <vector>

#include "lib/parser/ast/nodes/base/AstNode.hpp"
#include "lib/parser/ast/nodes/base/Decl.hpp"

class Module : public AstNode {
public:
  void Accept(AstVisitor& v) override;

  [[nodiscard]] std::vector<std::unique_ptr<Decl>>& GetDecls();
  [[nodiscard]] const std::vector<std::unique_ptr<Decl>>& GetDecls() const;

private:
  std::vector<std::unique_ptr<Decl>> decls_;
};

#endif // MODULE_HPP_
