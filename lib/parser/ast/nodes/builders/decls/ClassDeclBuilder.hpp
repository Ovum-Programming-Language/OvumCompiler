#ifndef PARSER_CLASSDECLBUILDER_HPP_
#define PARSER_CLASSDECLBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/ClassDecl.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class ClassDeclBuilder : public NodeBuilderBase<ClassDecl> {
public:
  ClassDeclBuilder& WithName(std::string name);
  ClassDeclBuilder& AddImplements(TypeReference iface);
  ClassDeclBuilder& AddMember(std::unique_ptr<Decl> member);
};

} // namespace ovum::compiler::parser

#endif // PARSER_CLASSDECLBUILDER_HPP_
