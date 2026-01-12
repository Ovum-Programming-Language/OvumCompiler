#ifndef PARSER_CLASSDECL_HPP_
#define PARSER_CLASSDECL_HPP_

#include <memory>
#include <string>
#include <vector>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class ClassDecl : public Decl {
public:
  void Accept(AstVisitor& visitor) override;

  [[nodiscard]] const std::string& Name() const noexcept;
  void SetName(std::string name);

  [[nodiscard]] const std::vector<TypeReference>& Implements() const noexcept;
  std::vector<TypeReference>& MutableImplements() noexcept;
  void AddImplements(TypeReference type);

  [[nodiscard]] const std::vector<std::unique_ptr<Decl>>& Members() const noexcept;
  std::vector<std::unique_ptr<Decl>>& MutableMembers() noexcept;
  void AddMember(std::unique_ptr<Decl> decl);
  std::unique_ptr<Decl> ReleaseMember(std::size_t index);

private:
  std::string name_;
  std::vector<TypeReference> implements_;
  std::vector<std::unique_ptr<Decl>> members_; // Field/StaticField/Method/Call/Destructor
};

} // namespace ovum::compiler::parser

#endif // PARSER_CLASSDECL_HPP_
