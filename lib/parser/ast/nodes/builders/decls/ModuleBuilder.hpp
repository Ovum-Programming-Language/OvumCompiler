#ifndef PARSER_MODULEBUILDER_HPP_
#define PARSER_MODULEBUILDER_HPP_

#include <memory>
#include <string>

#include "lib/parser/ast/nodes/base/Decl.hpp"
#include "lib/parser/ast/nodes/builders/base/NodeBuilderBase.hpp"
#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/tokens/SourceId.hpp"

namespace ovum::compiler::parser {

class ModuleBuilder : public NodeBuilderBase<Module> {
public:
  ModuleBuilder& WithName(std::string name);
  ModuleBuilder& WithSource(SourceId id);
  ModuleBuilder& AddDecl(std::unique_ptr<Decl> declaration);
};

} // namespace ovum::compiler::parser

#endif // PARSER_MODULEBUILDER_HPP_
