#include "lib/parser/states/StateModule.hpp"

#include <memory>

#include "lib/parser/ast/nodes/decls/Module.hpp"
#include "lib/parser/context/ContextParser.hpp"
#include "lib/parser/states/base/StateRegistry.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

std::string_view StateModule::Name() const {
  return "Module";
}

IState::StepResult StateModule::TryStep(ContextParser& context, ITokenStream& token_stream) const {
  if (context.NodeStack().empty()) {
    auto module = std::make_unique<Module>();
    context.PushNode(std::move(module));
  }

  if (token_stream.IsEof()) {
    return false;
  }

  context.PushState(StateRegistry::TopDecl());
  return true;
}

} // namespace ovum::compiler::parser
